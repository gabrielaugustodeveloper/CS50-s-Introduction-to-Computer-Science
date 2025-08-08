#include "helpers.h"  // Contains definitions for RGBTRIPLE and BYTE
#include <math.h>     // Provides round() function for proper rounding

// Convert image to grayscale by setting all color channels to average value
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate through each row (height dimension)
    for (int i = 0; i < height; i++)
    {
        // Process each pixel in current row (width dimension)
        for (int j = 0; j < width; j++)
        {
            // Access current pixel values (more efficient than repeated indexing)
            RGBTRIPLE pixel = image[i][j];

            // Calculate luminance (perceived brightness) using average method:
            // Average = (Red + Green + Blue) / 3
            // Using 3.0 forces floating-point division for accuracy
            BYTE average = round((pixel.rgbtRed + pixel.rgbtGreen + pixel.rgbtBlue) / 3.0);

            // Set all color channels to the calculated average
            // This creates a shade of gray ranging from black (0) to white (255)
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    return;  // Explicit return for clarity
}

// Apply sepia tone filter to give image antique brownish appearance
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate through each pixel in the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Store original values to prevent modification during calculation
            BYTE originalRed = image[i][j].rgbtRed;
            BYTE originalGreen = image[i][j].rgbtGreen;
            BYTE originalBlue = image[i][j].rgbtBlue;

            // Apply sepia transformation formulas (based on historical photographic process)
            // These coefficients mimic the effect of sepia toning chemicals
            float sepiaRed = .393 * originalRed + .769 * originalGreen + .189 * originalBlue;
            float sepiaGreen = .349 * originalRed + .686 * originalGreen + .168 * originalBlue;
            float sepiaBlue = .272 * originalRed + .534 * originalGreen + .131 * originalBlue;

            // Prevent color channel overflow (values must stay within 0-255 range)
            // Cap values at 255 to avoid wrapping/undefined behavior
            if (sepiaRed > 255) sepiaRed = 255;
            if (sepiaGreen > 255) sepiaGreen = 255;
            if (sepiaBlue > 255) sepiaBlue = 255;

            // Assign calculated values back to pixel with proper rounding
            // Rounding ensures closest integer representation
            image[i][j].rgbtRed = round(sepiaRed);
            image[i][j].rgbtGreen = round(sepiaGreen);
            image[i][j].rgbtBlue = round(sepiaBlue);
        }
    }
    return;
}

// Create horizontal mirror effect (flip image left-right)
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Process each row independently
    for (int i = 0; i < height; i++)
    {
        // Only need to process first half of pixels (middle stays if odd width)
        for (int j = 0; j < width / 2; j++)
        {
            // Calculate mirror position: right side counterpart to current left position
            // width-1 is last index (0-based), so width-1-j is symmetric opposite
            int mirror_j = width - 1 - j;

            // Classic swap using temporary variable:
            // 1. Save left pixel to temporary storage
            RGBTRIPLE temp = image[i][j];
            // 2. Replace left pixel with right pixel
            image[i][j] = image[i][mirror_j];
            // 3. Place saved left pixel in right position
            image[i][mirror_j] = temp;
        }
    }
    return;
}

// Apply box blur filter to create soft, out-of-focus effect
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create temporary image copy to preserve original values during processing
    // Essential because blur calculations need unmodified neighbor pixels
    RGBTRIPLE temp[height][width];

    // Copy entire image to temporary storage
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    // Process each pixel in the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Initialize accumulators as floats for precise averaging
            float totalRed = 0;    // Sum of red channel values
            float totalGreen = 0;  // Sum of green channel values
            float totalBlue = 0;   // Sum of blue channel values
            int count = 0;         // Number of valid neighbors counted

            // Examine 3x3 neighborhood centered at current pixel (i,j)
            for (int di = -1; di <= 1; di++)    // Vertical offset: -1 (up) to 1 (down)
            {
                for (int dj = -1; dj <= 1; dj++)  // Horizontal offset: -1 (left) to 1 (right)
                {
                    // Calculate neighbor coordinates
                    int ni = i + di;  // Neighbor row index
                    int nj = j + dj;  // Neighbor column index

                    // Check if neighbor exists (within image boundaries)
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        // Accumulate color values from valid neighbors
                        totalRed += temp[ni][nj].rgbtRed;
                        totalGreen += temp[ni][nj].rgbtGreen;
                        totalBlue += temp[ni][nj].rgbtBlue;
                        count++;  // Increment valid neighbor counter
                    }
                }
            }

            // Calculate box blur averages (sum / count) with proper rounding
            BYTE avgRed = round(totalRed / count);
            BYTE avgGreen = round(totalGreen / count);
            BYTE avgBlue = round(totalBlue / count);

            // Apply blurred values to original image
            image[i][j].rgbtRed = avgRed;
            image[i][j].rgbtGreen = avgGreen;
            image[i][j].rgbtBlue = avgBlue;
        }
    }
    return;
}
