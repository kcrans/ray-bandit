# ray-bandit

A CPU-based raytracing engine inspired from the [Ray Tracing in One Weekend](https://raytracing.github.io) book.

Here's what the final render from the above book looks lke:

![A scene consisting of randomly colored and spaced spheres. There are 3 larger spheres of each material type](/images/final_render.jpg)

For weird, buggy images check out the `/bloopers` directory.

## Getting Started

1. Go to `main.cpp` and construct the scene you want to render
2. Execute `make` on your target machine of choice.
3. Run `./raytracer`.
4. Input the file name for the rendered image and begin rendering. 
5. All images will be saved to `/images` in both ppm and jpeg formats.
