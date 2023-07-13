#ifndef _frame_h_
#define _frame_h_

#include <cstring>
#include "GL/glut.h"

#include "vec.h"

typedef unsigned char byte;

namespace kart
{
    class frame
    {
    public:
        int w, h;
        byte(*Image)[3];

        frame(const int& new_w, const int& new_h) :w(new_w), h(new_h), Image(new byte[w * h][3])
        {
            for (int i = 0; i < w * h; i++)
                for (int j = 0; j < 3; j++)
                    Image[i][j] = (unsigned char)0;
        };
        ~frame()
        {
            delete[] Image;
        };

        void put_pixel(int x, int y, vec color) const
        {
            x = (x + w) % w;
            y = (y + h) % h;
            auto clamp =
                [](dbl t)
            {
                if (t < 0)
                    return 0.0;
                if (t > 1)
                    return 1.0;
                return t;
            };

            int off = y * w + x;
            Image[off][0] = int(clamp(color.Z) * 255);
            Image[off][1] = int(clamp(color.Y) * 255);
            Image[off][2] = int(clamp(color.X) * 255);
        }

        void draw() const
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glRasterPos2d(-1, 1);
            glPixelZoom(1, -1);
            glDrawPixels(w, h, GL_BGR_EXT, GL_UNSIGNED_BYTE, Image);
        };
    };
}

#endif //_frame_h_