# QuadTree
Encodes an image by finding regions of pixels within an image that are of similar light intensity. 

A school project from October 2015
with partner Cheldon Coughlen

Usage (Linux):
```./quadTree lena.bmp 32```

try running this with different factors (3rd argument) to get different levels of compression with different data loss.
Run any image that is 512px x 512px 24-bit bmp 


If application needs to be compiled:
See if you have GLUT installed
```ls /usr/include/GL```

if glut.h is there you should be good.
Otherwise download:
```sudo apt-get install freeglut3-dev```

Make the application:
```make```




