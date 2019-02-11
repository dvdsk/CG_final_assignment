
#include "png_decode.h"

Image_seperate_channels load_channels_png(const char* filename){
    std::vector<uint8_t> rgba_array;
    Image_seperate_channels img;

    unsigned error = lodepng::decode(rgba_array, img.width, img.height, filename);
    if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    else {
        //the pixels are now in the vector "rgba_array", 4 bytes per pixel, ordered RGBARGBA

        //iterate over the pixels
        for(int i=0; i<img.width*img.height*4; i+=4){
            img.r.push_back(rgba_array[i+0]);
            img.g.push_back(rgba_array[i+1]);
            img.b.push_back(rgba_array[i+2]);
        }
    }
    std::cout << "loaded image: " << filename << std::endl;
    return img;
}

Image_rgb load_rgb_png(const char* filename){
    std::vector<uint8_t> rgba_array;
    Image_rgb img;

    unsigned error = lodepng::decode(rgba_array, img.width, img.height, filename);
    if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    else {
        //the pixels are now in the vector "rgba_array", 4 bytes per pixel, ordered RGBARGBA

        //iterate over the pixels
        for(int i=0; i<img.width*img.height*4; i+=4){
            img.rgb.push_back( (float)rgba_array[i+0]/255.);
            img.rgb.push_back( (float)rgba_array[i+1]/255.);
            img.rgb.push_back( (float)rgba_array[i+2]/255.);
        }
    }
    std::cout << "loaded image: " << filename << std::endl;
    return img;
}
