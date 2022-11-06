#include "helpers.h"
#include "math.h"

void pixel_to_grey(RGBTRIPLE *pixel);
void swap_pixel(RGBTRIPLE *right_pixel, RGBTRIPLE *left_pixel);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // RGBTRIPLE *pixel = &image[i][j];
            pixel_to_grey(&image[i][j]);
        }
    }
    return;
}

// Average Red, Green and Blue color.
void pixel_to_grey(RGBTRIPLE *pixel)
{
    int average_pixel = round(((*pixel).rgbtBlue + (*pixel).rgbtRed + (*pixel).rgbtGreen) / 3.);
    (*pixel).rgbtBlue = average_pixel;
    (*pixel).rgbtRed = average_pixel;
    (*pixel).rgbtGreen = average_pixel;
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            swap_pixel(&image[i][j], &image[i][width - j - 1]);
        }
    }
    return;
}

void swap_pixel(RGBTRIPLE *right_pixel, RGBTRIPLE *left_pixel)
{
    RGBTRIPLE buffer = *left_pixel;
    *left_pixel = *right_pixel;
    *right_pixel = buffer;
}


// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    RGBTRIPLE temp[height][width];

    // copy of the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    //iterate all pixels of the image.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int buffer_red = 0;
            int buffer_green = 0;
            int buffer_blue = 0;
            float n = 0;

            // iterate only the pixel area to average.
            for (int y = fmax(0, i - 1); y < fmin(height, i + 2); y++)
            {
                for (int x = fmax(0, j - 1); x < fmin(width, j + 2); x++)
                {
                    buffer_red   += temp[y][x].rgbtRed;
                    buffer_green += temp[y][x].rgbtGreen;
                    buffer_blue  += temp[y][x].rgbtBlue;
                    n++;
                }
            }

            image[i][j].rgbtRed   = round(buffer_red / n);
            image[i][j].rgbtGreen = round(buffer_green / n);
            image[i][j].rgbtBlue  = round(buffer_blue / n);
        }
    }
    return;
}


// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}
