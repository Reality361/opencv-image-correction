#include "QuadDetector.hpp"  // Include the header we just created

int main(int argc, char** argv)
{
    // Decide whether to read from an image or from camera
    bool useCamera = (argc < 2); // If no arguments, use camera

    cv::Mat frame;
    if (useCamera)
    {
        // --- CAMERA MODE ---
        cv::VideoCapture cap(0);
        if (!cap.isOpened())
        {
            std::cerr << "Error: Could not open the default camera.\n";
            return -1;
        }

        while (true)
        {
            cap.read(frame);
            if (frame.empty())
            {
                std::cerr << "Error: Failed to grab a frame from the camera.\n";
                break;
            }

            // Detect edges
            cv::Mat edges = QuadDetector::detectEdges(frame);

            // Warp the largest 4-corner region to a fixed size
            cv::Mat warped = QuadDetector::processLargestQuad(frame, edges, 500.0f, 600.0f);

            // Show the annotated original
            cv::imshow("Annotated Original", frame);

            // Show the warped output
            cv::imshow("Warped Output", warped);

            // Press 'q' or 'ESC' to exit
            char c = static_cast<char>(cv::waitKey(5));
            if (c == 'q' || c == 27)
            {
                break;
            }
        }
    }
    else
    {
        // --- IMAGE MODE ---
        std::string imagePath = argv[1];
        frame = cv::imread(imagePath);
        if (frame.empty())
        {
            std::cerr << "Error: Could not open or find the image at: " << imagePath << "\n";
            return -1;
        }

        // Detect edges
        cv::Mat edges = QuadDetector::detectEdges(frame);

        // Warp the largest 4-corner region
        cv::Mat warped = QuadDetector::processLargestQuad(frame, edges, 500.0f, 600.0f);

        // Show the annotated original
        cv::imshow("Annotated Original", frame);

        // Show the warped output
        cv::imshow("Warped Output", warped);

        // Wait for key press before exit (so windows doní»t close immediately)
        cv::waitKey(0);
    }

    return 0;
}
