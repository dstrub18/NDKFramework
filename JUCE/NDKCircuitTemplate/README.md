How to add Eigen to a JUCE project on mac.

1. Download Eigen using Brew

3. In the .jucer project file, go to "Project Settings" and "Header Search Paths" and add "/usr/local/include/eigen3/Eigen".

4. In your C++ file, add:


  #include <Eigen> 
