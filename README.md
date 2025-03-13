# 📄 OpenCV Image Correction for Documents

## ✨ Overview
**OpenCV Image Correction** is a C++ project designed to automatically detect and correct skewed or distorted document images. This project leverages OpenCV to detect quadrilateral regions (such as documents) and warp them into a properly aligned view.

## 🚀 Features
✅ **Automatic Document Detection** - Detects quadrilateral shapes in an image and corrects perspective.
✅ **Edge Detection** - Uses OpenCV to detect edges and find the document boundaries.
✅ **Camera & Image Input Support** - Works with both pre-captured images and live camera feed.
✅ **Real-Time Processing** - Processes and displays corrected document images in real time.

## 🏗️ Project Structure
```
📂 opencv-image-correction
 ├── 📄 main.cpp          # Main entry point of the project
 ├── 📄 QuadDetector.hpp  # Header file for quadrilateral detection and processing
 ├── 📄 README.md         # Project documentation
```

## 🔧 Installation & Setup
### Prerequisites
Make sure you have the following installed on your system:
- **C++ Compiler (g++)**
- **OpenCV (>=4.0.0)**
- **CMake**

### Build Instructions
1. Clone this repository:
   ```sh
   git clone https://github.com/Reality361/opencv-image-correction.git
   cd opencv-image-correction
   ```
2. Create a build directory and compile the project:
   ```sh
   mkdir build && cd build
   cmake ..
   make
   ```
3. Run the application:
   ```sh
   ./DocumentImageCorrection <image_path>
   ```
   OR
   ```sh
   ./DocumentImageCorrection  # Runs in camera mode
   ```

## 📸 Usage
- If you provide an **image file path**, the program will detect and correct the document in that image.
- If **no arguments** are given, the program will open the default camera and process document images in real time.
- Press `q` or `ESC` to exit in camera mode.

## 🖼️ Example Output
Original Image     /     Corrected Image
---
![158a6c56b30fa72266e72dc111b6d2b](https://github.com/user-attachments/assets/0932dbac-d544-4174-8a0f-9c111c3c4ff1)


## 🛠️ Technologies Used
- **C++** - Core language
- **OpenCV** - Image processing
- **CMake** - Build system

## 👥 Contributors
- **Reality361** - [GitHub](https://github.com/Reality361)
- Contributions welcome! Feel free to submit pull requests.

## 📜 License
This project is licensed under the **MIT License**.

---
Give this repository a ⭐ if you found it helpful!

