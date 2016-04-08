
#pragma once
#include <QWidget>

namespace Ps{
    class Tile
    {
    public:
        Tile(long x = 0, long y = 0, long z = 0);

        long tx;
        long ty;
        long zoom;

    private:
//        explicit Tile(const Tile& rhs) = delete;
//        Tile& operator= (const Tile& rhs) = delete;
    };
}


