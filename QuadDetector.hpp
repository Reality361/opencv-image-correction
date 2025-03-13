#pragma once

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace QuadDetector
{
    /**
     * \brief Sorts the given 4 points so that:
     *        [0] -> top-left, [1] -> top-right
     *        [2] -> bottom-left, [3] -> bottom-right
     */
    inline std::vector<cv::Point> sortCorners(const std::vector<cv::Point>& corners)
    {
        std::vector<cv::Point> sorted = corners;

        // Sort by Y coordinate (lowest Y first)
        for (int i = 0; i < 4; i++)
        {
            for (int j = i + 1; j < 4; j++)
            {
                if (sorted[i].y > sorted[j].y)
                {
                    std::swap(sorted[i], sorted[j]);
                }
            }
        }

        // Among the top two, ensure [0].x < [1].x
        if (sorted[0].x > sorted[1].x)
        {
            std::swap(sorted[0], sorted[1]);
        }

        // Among the bottom two, ensure [2].x < [3].x
        if (sorted[2].x > sorted[3].x)
        {
            std::swap(sorted[2], sorted[3]);
        }

        return sorted;
    }

    /**
     * \brief Performs edge detection on an input image using
     *        grayscale, Gaussian blur, Canny, and dilation.
     */
    inline cv::Mat detectEdges(const cv::Mat& input)
    {
        cv::Mat gray, blurred, edges, dilated;

        // Convert to grayscale
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);

        // Blur to reduce noise
        cv::GaussianBlur(gray, blurred, cv::Size(3, 3), 3.0, 0.0);

        // Canny edge detection
        cv::Canny(blurred, edges, 70, 90);

        // Dilation to strengthen edges
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
        cv::dilate(edges, dilated, kernel);

        return dilated;
    }

    /**
     * \brief Finds the largest 4-corner contour from the edge image, warps it,
     *        draws the largest quadrilateral on the original, and marks corners.
     *
     * \param original   The original color image (for drawing and warping).
     * \param edges      The edge image (for finding contours).
     * \param warpWidth  The desired width of the warped output.
     * \param warpHeight The desired height of the warped output.
     * \return           The perspective-warped image of the largest 4-corner contour
     *                   or the original if none is found.
     */
    inline cv::Mat processLargestQuad(
        cv::Mat& original,
        const cv::Mat& edges,
        float warpWidth = 500.0f,
        float warpHeight = 600.0f)
    {
        // Find external contours
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(edges, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        std::cout << "Number of contours found: " << contours.size() << std::endl;

        // Track largest 4-corner contour
        int largestArea = 0;
        int bestContourIndex = -1;

        // Approximate each contour; check for 4 corners and largest area
        std::vector<std::vector<cv::Point>> approxPolys(contours.size());
        for (size_t i = 0; i < contours.size(); ++i)
        {
            float perimeter = cv::arcLength(contours[i], true);
            cv::approxPolyDP(contours[i], approxPolys[i], 0.02f * perimeter, true);

            int area = static_cast<int>(cv::contourArea(approxPolys[i]));
            if (approxPolys[i].size() == 4 && area > largestArea)
            {
                largestArea = area;
                bestContourIndex = static_cast<int>(i);
            }
        }

        // If we found no 4-corner contour, just return the original as-is
        if (bestContourIndex == -1)
        {
            std::cout << "No valid 4-corner contour found.\n";
            return original;
        }

        // Sort the corners for consistent perspective transform
        std::vector<cv::Point> corners = sortCorners(approxPolys[bestContourIndex]);

        // Create warp matrix
        cv::Point2f src[4] = {
            corners[0],
            corners[1],
            corners[2],
            corners[3]
        };

        cv::Point2f dst[4] = {
            cv::Point2f(0, 0),
            cv::Point2f(warpWidth, 0),
            cv::Point2f(0, warpHeight),
            cv::Point2f(warpWidth, warpHeight)
        };

        cv::Mat transformMatrix = cv::getPerspectiveTransform(src, dst);
        cv::Mat warped;
        cv::warpPerspective(
            original,
            warped,
            transformMatrix,
            cv::Size(static_cast<int>(warpWidth), static_cast<int>(warpHeight))
        );

        // Draw the largest 4-corner contour on the original
        cv::drawContours(original, approxPolys, bestContourIndex, cv::Scalar(0, 255, 0), 3);

        // Mark the corners on the original
        for (int i = 0; i < 4; ++i)
        {
            cv::circle(original, corners[i], 8, cv::Scalar(255, 0, 0), cv::FILLED);
            cv::putText(original,
                std::to_string(i + 1),
                corners[i],
                cv::FONT_HERSHEY_COMPLEX,
                0.7,
                cv::Scalar(0, 255, 0),
                2);
        }

        return warped;
    }
} // end namespace QuadDetector
