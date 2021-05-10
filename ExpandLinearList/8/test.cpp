#include "BMP.h"

int main()
{
    BMP lena("lena.bmp");
    // lena.save("lena_copy.bmp");
    // BMP lena_gray(lena.to8Gray());
    // lena_gray.save("lena_gray.bmp");
    // BMP lena_gray24(lena.to24Gray());
    // lena_gray24.save("lena)gray24.bmp");

    // lena_gray24.smooth().save("lena_gray24_smooth.bmp");
    // lena_gray.smooth().save("lena_gray_smooth.bmp");

    // lena.luminance(0.2).save("lena_lighted.bmp");
    // lena.contrast(0.4).save("lena_con_04.bmp");
    // lena.contrast(-0.4).save("lena_con_n04.bmp");
    lena.to8Gray().gray2BnW().save("lena_bnw.bmp");
    return 0;
}
