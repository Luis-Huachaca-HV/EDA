
//#ifdef __APPLE__
//#include <GLUT/glut.h>
//#include <GL/freeglut.h>
#include <GL/glut.h>

//#else

//#endif
#include <cstddef>
#include <iostream>
#include "loadBMP.cpp"
#include "quadtree.cpp"
double zoom = 1.0;            // Pixel zoom, for both x & y
const double maxzoom = 10.0;
const double minzoom = -10.0;


// A pixel struct for holding pixel data.
struct pixel
{
    char r, g, b;

    bool operator == ( const pixel & rhs ) const
    {
        return ( r == rhs.r && g == rhs.g && b == rhs.b );
    }

    bool operator != ( const pixel & rhs ) const
    {
        return ( r != rhs.r || g != rhs.g || b != rhs.b );
    }
};


bool DRAW_LINES = false;
double QUALITY = 100;
quadtree<pixel> * QUAD_IMAGE;
char * ORIGINAL_IMAGE, * COMPRESSED_IMAGE;
size_t IMAGE_WIDTH, IMAGE_HEIGHT;



void printUsageInstructions()
{
    std::cout <<
              "This program requires one or two arguments be passed to it.\n"
              "ex: <program name> <argument 1> [argument 2]\n\n"

              "Argument 1 :\n"
              "The first argument must be a .bmp image somewhere on your computer. This image\n"
              "is the image that will be used by the program.\n\n"

              "Argument 2 ( optional ) :\n"
              "The second argument must be a number (integer or real) between 0 and 100\n"
              "specifying the quality factor used by the program. A value of 100 is lossless\n"
              "quality, and a value of 0 will compress the image to a single color. If this\n"
              "argument is not specified, it will be set to 100.\n\n"

              "Press 'Enter' to exit the program.";
}


template<class datatype>
datatype checkval( const datatype ** const image,
                   const size_t x1, const size_t x2, const size_t y1,
                   const size_t y2, const datatype & eps )
{
    size_t i, j, height, width = 0;
    datatype curr, sum, min, max, avg = 0;
    height = y2 - y1;
    width = x2 - x1;

    for ( i = x1; i < x2; ++i )
    {
        for ( j = y1; j < y2; ++j )
        {
            curr = image[i][j];
            sum += curr;
            if ( max < curr )
                max = curr;
            if ( min > curr )
                min = curr;
            if ( i == height - 1 && j == width - 1 )
                avg = sum / ( height * width );
            if ( max - min > 0 && max - min > 2 * eps ) //worst case reached
                return 0;
        }
    }
    std::cout <<"desv_estandar" <<eps << std::endl;
    if ( max - min < 2 * eps ){
        std::cout <<"desv_estandar" <<eps << std::endl;
        return avg;}
    else{

    std::cout <<"desv_estandar" <<eps << std::endl;
        return (datatype) -1;}

}


template<class datatype>
void subdiv( quadtree<datatype> & tree, const datatype ** const image,
             const size_t x1, const size_t x2, const size_t y1, const size_t y2,
             const datatype & eps )
{
    const datatype avg = checkval( image, x1, x2, y1, y2, eps );
    std::cout <<"desv_estandar" <<eps << std::endl;

    if ( avg != (datatype) -1 ) //if pass checkval pass the average value as data
        tree.insert( x1, y1, avg );

    else //if fails the checkval
    {
        subdiv( tree, image, x1, x2 / 2, y1, y2 / 2, eps ); //tl
        subdiv( tree, image, x2 / 2, x2, y1, y2 / 2, eps ); //tr
        subdiv( tree, image, x1, x2 / 2, y2 / 2, y2, eps ); //bl
        subdiv( tree, image, x2 / 2, x2, y2, y2, eps ); //br
    }
}

bool loadImage( const char * const filename )
{
    if ( loadBMP( filename, IMAGE_WIDTH, IMAGE_HEIGHT, ORIGINAL_IMAGE ) )
    {

        QUAD_IMAGE = new quadtree<pixel>( IMAGE_WIDTH, IMAGE_HEIGHT );


        for ( size_t y = 0; y < IMAGE_HEIGHT; ++y )
        {
            for ( size_t x = 0; x < IMAGE_WIDTH; ++x )
            {
                const pixel temp = { ORIGINAL_IMAGE[IMAGE_WIDTH*y+3*x],
                                     ORIGINAL_IMAGE[IMAGE_WIDTH*y+3*x+1],
                                     ORIGINAL_IMAGE[IMAGE_WIDTH*y+3*x+2] };

                QUAD_IMAGE->insert( x, y, temp );
            }
        }


        std::cout << "Size of BMP image: "
                  << 3 * IMAGE_WIDTH * IMAGE_HEIGHT * sizeof( ORIGINAL_IMAGE ) << "\n\n";

        std::cout << "Before optimize() is called\n---------------------------------------\n"
                  << "quadtree size in bytes:       " << QUAD_IMAGE->size() << '\n'
                  << "number of quads in the tree:  " << QUAD_IMAGE->numQuads() << '\n'
                  << "number of points in the tree: " << QUAD_IMAGE->numPoints() << "\n\n";

        QUAD_IMAGE->optimize();


        std::cout << "After optimize() is called\n--------------------------------------\n"
                  << "quadtree size in bytes:       " << QUAD_IMAGE->size() << '\n'
                  << "number of quads in the tree:  " << QUAD_IMAGE->numQuads() << '\n'
                  << "number of points in the tree: " << QUAD_IMAGE->numPoints() << "\n\n";


        COMPRESSED_IMAGE = new char[IMAGE_WIDTH*IMAGE_HEIGHT];

        for ( size_t y = 0; y < IMAGE_HEIGHT; ++y )
        {
            for ( size_t x = 0; x < IMAGE_WIDTH; ++x )
            {


                const pixel temp = QUAD_IMAGE->getData( x, y );
                COMPRESSED_IMAGE[IMAGE_WIDTH*y+3*x] = temp.r;
                COMPRESSED_IMAGE[IMAGE_WIDTH*y+3*x+1] = temp.g;
                COMPRESSED_IMAGE[IMAGE_WIDTH*y+3*x+2] = temp.b;
                //QUAD_IMAGE->insert( x, y, temp );
            }
        }


        return true;
    }

    else return false;
}


void drawImages()
{
    glRasterPos2i( 0, 0 );
    glPixelZoom(zoom, zoom);
   glDrawPixels( IMAGE_WIDTH, IMAGE_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, ORIGINAL_IMAGE );

    //glRasterPos2i( +150, 0 );
    //glDrawPixels( IMAGE_WIDTH, IMAGE_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, COMPRESSED_IMAGE );

    //glDrawPixels( IMAGE_WIDTH, IMAGE_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, COMPRESSED_IMAGE );
}


void display()
{
    // Clear Window
    glClear( GL_COLOR_BUFFER_BIT );
    glLoadIdentity();
    glOrtho( 0, glutGet( GLUT_WINDOW_WIDTH ), 0, glutGet( GLUT_WINDOW_HEIGHT ), -1, 1 );

    // Draw Images
    drawImages();

    // Draw Quadtree Line

    glFinish();
}




void test()
{
    quadtree<int> test( 500, 500 );

    test.insert( 0, 0, 0 );
    test.insert( 0, 0, 1 );
    test.insert( 0, 1, 1 );
    test.insert( 0, 2, 1 );
    test.insert( 1, 0, 1 );
    test.insert( 1, 1, 1 );
    test.insert( 1, 2, 1 );
    test.insert( 2, 0, 1 );
    test.insert( 2, 1, 1 );
    test.insert( 2, 2, 1 );

    std::cout << "quadtree size in bytes:       " << test.size() << '\n'
              << "number of quads in the tree:  " << test.numQuads() << '\n'
              << "number of points in the tree: " << test.numPoints() << "\n\n";

    test.optimize();


    std::cout << "quadtree size in bytes:       " << test.size() << '\n'
              << "number of quads in the tree:  " << test.numQuads() << '\n'
              << "number of points in the tree: " << test.numPoints() << "\n\n";
}


int main( int argc, char * argv[] )
{
    //int a = a;
    test();

    if ( argc < 2 || argc > 3 )
    {
        printUsageInstructions();

        std::cin.ignore();

        return 0;
    }

    if ( !loadImage( argv[1] ) ) return 0;

    // Glut Init Functions
    glutInitWindowSize( 600, 600 );
    glutInitWindowPosition( 0, 0 );
    glutInitDisplayMode( GLUT_RGB );
    glutInit( &argc, argv );


    glutCreateWindow( "Quadtree Image Encoding" );

    // GL Init Functions
    glClearColor( 0, 0, 0, 0 ); // Black
    glMatrixMode( GL_PROJECTION );

    // Glut Event Functions
    glutDisplayFunc( display );

    glutMainLoop();

    delete[] QUAD_IMAGE;
    delete[] COMPRESSED_IMAGE;

}
