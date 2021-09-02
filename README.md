# QURScanner

QURScanner is a Qt widget whose main purpose is to demonstrate the receiving of [UR ("Uniform Resources")](https://github.com/BlockchainCommons/Research/blob/master/papers/bcr-2020-005-ur.md) data embedded in QR codes. It utilizes the QCamera for capturing frames in which the QR codes are detected and decoded using the OpenCV's [wechat_qrcode](https://github.com/opencv/opencv_contrib/tree/master/modules/wechat_qrcode) module. Only the QR codes that contain UR data are processed further. The UR data are decoded using the [bc-ur](https://github.com/PavelNajman/bc-ur) library, and their hashes are computed and visualized using the [bc-lifehash](https://github.com/PavelNajman/bc-lifehash) library. The widget supports both single-part and multi-part URs.

## Getting started
The demo can be built using either qmake
```
mkdir build && cd build
qmake ..
make
```
or CMake.
```
mkdir build && cd build
cmake ..
make
```
For the best results, put the [CNN detector and super scale model](https://github.com/WeChatCV/opencv_3rdparty/tree/wechat_qrcode) files to the directory from which you would run the executable.

## Additional information
The demo application consists of two screens - scan and results. The scan screen is displayed first. It contains a combobox for the camera selection and a label that displays the camera frames. During the scanning of the multi-part URs, a progress bar is displayed at the bottom. When all the parts of the UR are scanned, the results screen is displayed. 

![Scan screen](/images/scan_screen.png)

The results screen contains a text browser with the lifehash image of the decoded data and a list of the encoded URs. At the bottom of the results screen is a button which starts a new scan.

![Results screen](/images/results_screen.png)
