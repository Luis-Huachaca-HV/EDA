//
// Created by luishuachaca on 9/28/22.
//
#ifndef QUADTREE_H
#define QUADTREE_H
#include <cstddef>

template<class datatype>
class quadtree
{
private:

    struct point
    {
        point();
        point( const size_t X, const size_t Y, const datatype & D );

        datatype getData( const size_t X, const size_t Y ) const;
        bool contains( const size_t x, const size_t y ) const;

        virtual size_t insert( const point * const P ) { return 0; }
        virtual size_t insert( const size_t X, const size_t Y, const datatype & D ) { return 0; }
        virtual size_t optimize( size_t & numQuads, size_t & numPoints ) { return 0; }

        size_t x = 0, y = 0; // The location of this point.
        datatype data; // The data at this point.

    };

    struct quad : point
    {
        quad( const size_t X, const size_t Y, const size_t W, const size_t H, const datatype & D );
        ~quad();

        datatype getData( const size_t X, const size_t Y ) const;
        bool contains( const size_t x, const size_t y ) const;

        size_t insert( const point * const P );
        size_t insert( const size_t X, const size_t Y, const datatype & D );
        size_t optimize( size_t & numQuads, size_t & numPoints );

        // Top Left, Top Right, Bottom Left, Bottom Right
        point * tl = nullptr;
        point * tr = nullptr;
        point * bl = nullptr;
        point * br = nullptr;
        // The horizontal and vertical ranges of points this quad contains.
        size_t width, height;

    };

    quad * root = nullptr;

    // The horizontal and vertical ranges of points this quadtree contains.
    size_t width, height;

    // The number of quad and point structs currently in the tree.
    struct { size_t quads, points; } count = { 0, 0 };


public:

    quadtree( const size_t W, const size_t H );
    ~quadtree();
    bool insert( const size_t X, const size_t Y, const datatype & D );
    datatype getData( const size_t X, const size_t Y ) const;
    void optimize();
    void clear();
    size_t numQuads();
    size_t numPoints();
    size_t size();
};

template<class datatype>
quadtree<datatype>::point::point() {}

template<class datatype>
quadtree<datatype>::point::point( const size_t X, const size_t Y, const datatype & D )
        : x( X ), y( Y ), data( D ) {}

template<class datatype>
datatype quadtree<datatype>::point::getData( const size_t X, const size_t Y ) const { return data; }

template<class datatype>
bool quadtree<datatype>::point::contains( const size_t X, const size_t Y ) const
{
    return ( x == X && y == Y );
}
template<class datatype>
quadtree<datatype>::quad::quad( const size_t X, const size_t Y, const size_t W, const size_t H, const datatype & D )
        : point( X, Y, D ), width( W ), height( H ) {}

template<class datatype>
quadtree<datatype>::quad::~quad()
{
    delete tl;
    delete tr;
    delete bl;
    delete br;
}

template<class datatype>
datatype quadtree<datatype>::quad::getData( const size_t X, const size_t Y ) const
{
    quad * temp;
    if ( temp == dynamic_cast<quad*>( tl ) )
    {
        if ( temp->contains( X, Y ) )
            return temp->getData( X, Y );
    }
    else if ( tl != nullptr && tl->contains( X, Y ) ) return tl->data;
    if ( temp == dynamic_cast<quad*>( tr ) )
    {
        if ( temp->contains( X, Y ) )
            return temp->getData( X, Y );
    }
    else if ( tr != nullptr && tr->contains( X, Y ) ) return tr->data;
    if ( temp == dynamic_cast<quad*>( bl ) )
    {
        if ( temp->contains( X, Y ) )
            return temp->getData( X, Y );
    }
    else if ( bl != nullptr && bl->contains( X, Y ) ) return bl->data;
    if ( temp == dynamic_cast<quad*>( br ) )
    {
        if ( temp->contains( X, Y ) )
            return temp->getData( X, Y );
    }
    else if ( br != nullptr && br->contains( X, Y ) ) return br->data;

    // The point is not in any of the set quadrants of this quad, so it must
    // have the same data as this quads default data.
    return this->data;
}
template<class datatype>
bool quadtree<datatype>::quad::contains( const size_t X, const size_t Y ) const
{
    return (X >= this->x && X < (this->x + width ) && Y >= this->y && Y < (this->y + height ) );
}
template<class datatype>
size_t quadtree<datatype>::quad::insert( const point * const P )
{
    return ( P == nullptr ? -1 : insert( P->x, P->y, P->data ) );
}
template<class datatype>
size_t quadtree<datatype>::quad::insert( const size_t X, const size_t Y, const datatype & D )
{
    // Top
    if ( Y - this->y < height / 2 )
    {
        // Left
        if ( X - this->x < width / 2 )
        {
            if ( tl == nullptr ) tl = new point( X, Y, D );


            else if ( !dynamic_cast<quad*>(tl) )
            {
                // If it's actually the same point, just overwrite its data.
                if ( tl->contains( X, Y ) )
                {
                    tl->data = D;

                    return -1;
                }
                const point * const temp = tl;
                tl = new quad( this->x, this->y, width / 2, height / 2, this->data );

                tl->insert( temp ); // This will return 0.
                delete temp; // A new point was created from temp's data, so we can delete this one.
                return 1 + tl->insert( X, Y, D );
            }

            else if ( D != tl->data ) return tl->insert( X, Y, D );
        }

        else
        {
            if ( tr == nullptr ) tr = new point( X, Y, D );

            else if ( !dynamic_cast<quad*>( tr ) )
            {
                if ( tr->contains( X, Y ) )
                {
                    tr->data = D;

                    return -1;
                }
                const point * const temp = tr;
                tr = new quad( this->x + width / 2, this->y, width - width / 2, height / 2, this->data );

                tr->insert( temp );
                delete temp;
                return 1 + tr->insert( X, Y, D );
            }
            else if ( D != tr->data ) return tr->insert( X, Y, D );
        }
    }

    else
    {
        if ( X - this->x < width / 2 )
        {
            if ( bl == nullptr ) bl = new point( X, Y, D );

            else if ( !dynamic_cast<quad*>( bl ) )
            {
                if ( bl->contains( X, Y ) )
                {
                    bl->data = D;
                    return -1;
                }
                const point * const temp = bl;
                bl = new quad( this->x, this->y + height / 2, width / 2, height - height / 2, this->data );
                bl->insert( temp );
                delete temp;
                return 1 + bl->insert( X, Y, D );
            }
            else if ( D != bl->data ) return bl->insert( X, Y, D );
        }
        else
        {
            if ( br == nullptr ) br = new point( X, Y, D );
            else if ( !dynamic_cast<quad*>( br ) )
            {
                if ( br->contains( X, Y ) )
                {
                    br->data = D;

                    return -1;
                }
                const point * const temp = br;
                br = new quad( this->x + width / 2, this->y + height / 2, width - width / 2, height - height / 2, this->data );
                br->insert( temp );
                delete temp;
                return 1 + br->insert( X, Y, D );
            }

            else if ( D != br->data ) return br->insert( X, Y, D );
        }
    }

    return 0;
}

// modifica los subcuadrados
template <class datatype>
size_t quadtree<datatype>::quad::optimize( size_t & numQuads, size_t & numPoints )
{
    struct { size_t tl, tr, bl, br; } matchCount = { 0, 0, 0, 0 };

    if ( tl != nullptr )
    {
        // If the top left quad pointer points to a quad, optimize it.
        if ( dynamic_cast<quad*>( tl ) ) matchCount.tl = tl->optimize( numQuads, numPoints );

            // Otherwise it points to a point.
        else ++matchCount.tl;
    }
    if ( tr != nullptr )
    {
        if ( dynamic_cast<quad*>( tr ) ) matchCount.tr = tr->optimize( numQuads, numPoints );
        else ++matchCount.tr;
    }
    if ( bl != nullptr )
    {
        if ( dynamic_cast<quad*>( bl ) ) matchCount.bl = bl->optimize( numQuads, numPoints );
        else ++matchCount.bl;
    }
    if ( br != nullptr )
    {
        if ( dynamic_cast<quad*>( br ) ) matchCount.br = br->optimize( numQuads, numPoints );
        else ++matchCount.br;
    }
    const size_t numDefault = ( width * height ) - matchCount.tl - matchCount.tr - matchCount.bl - matchCount.br;
    datatype * items[5];
    size_t iCount[5] = { numDefault };
    if ( numDefault ) items[0] = &this->data;
    if ( tl != nullptr )
    {
        for ( size_t item = 0; item < 5; ++item )
        {
            if ( iCount[item] )
            {
                if ( *items[item] == tl->data )
                {
                    iCount[item] += matchCount.tl;
                    break;
                }
            }
            else
            {
                items[item] = &tl->data;
                iCount[item] = matchCount.tl;
                break;
            }
        }
    }
    if ( tr != nullptr )
    {
        for ( size_t item = 0; item < 5; ++item )
        {
            if ( iCount[item] )
            {
                if ( *items[item] == tr->data )
                {
                    iCount[item] += matchCount.tr;
                    break;
                }
            }
            else
            {
                items[item] = &tr->data;
                iCount[item] = matchCount.tr;
                break;
            }
        }
    }
    if ( bl != nullptr )
    {
        for ( size_t item = 0; item < 5; ++item )
        {
            if ( iCount[item] )
            {
                if ( *items[item] == bl->data )
                {
                    iCount[item] += matchCount.bl;
                    break;
                }
            }
            else
            {
                items[item] = &bl->data;
                iCount[item] = matchCount.bl;
                break;
            }
        }
    }
    if ( br != nullptr )
    {
        for ( size_t item = 0; item < 5; ++item )
        {
            if ( iCount[item] )
            {
                if ( *items[item] == br->data )
                {
                    iCount[item] += matchCount.br;
                    break;
                }
            }
            else
            {
                items[item] = &br->data;
                iCount[item] = matchCount.br;
                break;
            }
        }
    }
    datatype best = *items[0];
    size_t max = iCount[0];
    for ( size_t item = 1; item < 5; ++item )
    {
        if ( iCount[item] > max )
        {
            max = iCount[item];

            best = *items[item];
        }
    }
    if ( best != this->data )
    {
        const bool isSmallest = ( width < 5 && height < 5 );
        if ( !dynamic_cast<quad*>( tl ) )
        {
            if ( isSmallest )
            {
                if ( tl != nullptr && tl->data == best )
                {
                    delete tl;
                    tl = nullptr;
                    --numPoints;
                }
            }
            else
            {
                const point * const temp = tl;
                tl = new quad( this->x, this->y, width / 2, height / 2, this->data );
                tl->insert( temp );
                delete temp;
                ++numQuads;
            }
        }

        else if ( tl->data == best )
        {
            const quad * const temp = static_cast<quad*>( tl );
            if ( temp->tl == nullptr && temp->tr == nullptr && temp->bl == nullptr && temp->br == nullptr )
            {
                delete temp;
                tl = nullptr;
                --numQuads;
            }
            else if ( temp->tl != nullptr && temp->tr != nullptr && temp->bl != nullptr && temp->br != nullptr )
            {
                bool isPoint = true;
                if ( temp->tl != nullptr && !dynamic_cast<quad*>( temp->tl ) )
                    tl = new point( temp->tl->x, temp->tl->y, temp->tl->data );
                else if ( temp->tr != nullptr && !dynamic_cast<quad*>( temp->tr ) )
                    tl = new point( temp->tr->x, temp->tr->y, temp->tr->data );
                else if ( temp->bl != nullptr && !dynamic_cast<quad*>( temp->bl ) )
                    tl = new point( temp->bl->x, temp->bl->y, temp->bl->data );
                else if ( temp->br != nullptr && !dynamic_cast<quad*>( temp->br ) )
                    tl = new point( temp->br->x, temp->br->y, temp->br->data );
                else isPoint = false;

                if ( isPoint )
                {
                    delete temp;
                    --numQuads;
                }
            }
        }
        if ( !dynamic_cast<quad*>( tr ) )
        {
            if ( isSmallest )
            {
                if ( tr != nullptr && tr->data == best )
                {
                    delete tr;
                    tr = nullptr;
                    --numPoints;
                }
            }
            else
            {
                const point * const temp = tr;
                tr = new quad( this->x, this->y, width / 2, height / 2, this->data );
                tr->insert( temp );
                delete temp;
                ++numQuads;
            }
        }
        else if ( tr->data == best )
        {
            const quad * const temp = static_cast<quad*>( tr );
            if ( temp->tl == nullptr && temp->tr == nullptr && temp->bl == nullptr && temp->br == nullptr )
            {
                delete temp;
                tr = nullptr;
                --numQuads;
            }
            else if ( temp->tl != nullptr && temp->tr != nullptr && temp->bl != nullptr && temp->br != nullptr )
            {
                bool isPoint = true;
                if ( temp->tl != nullptr && !dynamic_cast<quad*>( temp->tl ) )
                    tr = new point( temp->tl->x, temp->tl->y, temp->tl->data );
                else if ( temp->tr != nullptr && !dynamic_cast<quad*>( temp->tr ) )
                    tr = new point( temp->tr->x, temp->tr->y, temp->tr->data );
                else if ( temp->bl != nullptr && !dynamic_cast<quad*>( temp->bl ) )
                    tr = new point( temp->bl->x, temp->bl->y, temp->bl->data );
                else if ( temp->br != nullptr && !dynamic_cast<quad*>( temp->br ) )
                    tr = new point( temp->br->x, temp->br->y, temp->br->data );
                else isPoint = false;
                if ( isPoint )
                {
                    delete temp;
                    --numQuads;
                }
            }
        }

        if ( !dynamic_cast<quad*>( bl ) )
        {
            if ( isSmallest )
            {
                if ( bl != nullptr && bl->data == best )
                {
                    delete bl;
                    bl = nullptr;
                    --numPoints;
                }
            }

            else
            {
                const point * const temp = bl;
                bl = new quad( this->x, this->y, width / 2, height / 2, this->data );
                bl->insert( temp );
                delete temp;
                ++numQuads;
            }
        }
        else if ( bl->data == best )
        {
            const quad * const temp = static_cast<quad*>( bl );

            if ( temp->tl == nullptr && temp->tr == nullptr && temp->bl == nullptr && temp->br == nullptr )
            {
                delete temp;
                bl = nullptr;
                --numQuads;
            }
            else if ( temp->tl != nullptr && temp->tr != nullptr && temp->bl != nullptr && temp->br != nullptr )
            {
                bool isPoint = true;

                if ( temp->tl != nullptr && !dynamic_cast<quad*>( temp->tl ) )
                    bl = new point( temp->tl->x, temp->tl->y, temp->tl->data );

                else if ( temp->tr != nullptr && !dynamic_cast<quad*>( temp->tr ) )
                    bl = new point( temp->tr->x, temp->tr->y, temp->tr->data );

                else if ( temp->bl != nullptr && !dynamic_cast<quad*>( temp->bl ) )
                    bl = new point( temp->bl->x, temp->bl->y, temp->bl->data );

                else if ( temp->br != nullptr && !dynamic_cast<quad*>( temp->br ) )
                    bl = new point( temp->br->x, temp->br->y, temp->br->data );

                else isPoint = false;

                if ( isPoint )
                {
                    delete temp;
                    --numQuads;
                }
            }
        }
        if ( !dynamic_cast<quad*>( br ) )
        {
            if ( isSmallest )
            {
                if ( br != nullptr && br->data == best )
                {
                    delete br;
                    br = nullptr;
                    --numPoints;
                }
            }
            else
            {
                const point * const temp = br;
                br = new quad( this->x, this->y, width / 2, height / 2, this->data );
                br->insert( temp );
                delete temp;
                ++numQuads;
            }
        }
        else if ( br->data == best )
        {
            const quad * const temp = static_cast<quad*>( br );

            if ( temp->tl == nullptr && temp->tr == nullptr && temp->bl == nullptr && temp->br == nullptr )
            {
                delete temp;
                br = nullptr;
                --numQuads;
            }
            else if ( temp->tl != nullptr && temp->tr != nullptr && temp->bl != nullptr && temp->br != nullptr )
            {
                bool isPoint = true;

                if ( temp->tl != nullptr && !dynamic_cast<quad*>( temp->tl ) )
                    br = new point( temp->tl->x, temp->tl->y, temp->tl->data );

                else if ( temp->tr != nullptr && !dynamic_cast<quad*>( temp->tr ) )
                    br = new point( temp->tr->x, temp->tr->y, temp->tr->data );

                else if ( temp->bl != nullptr && !dynamic_cast<quad*>( temp->bl ) )
                    br = new point( temp->bl->x, temp->bl->y, temp->bl->data );

                else if ( temp->br != nullptr && !dynamic_cast<quad*>( temp->br ) )
                    br = new point( temp->br->x, temp->br->y, temp->br->data );

                else isPoint = false;

                if ( isPoint )
                {
                    delete temp;
                    --numQuads;
                }
            }
        }
        this->data = best;
    }
    return max;
}

template<class datatype>
quadtree<datatype>::quadtree( const size_t W, const size_t H ) : width( W ), height( H ) {}

template <class datatype>
quadtree<datatype>::~quadtree()
{
    delete root;
}
template <class datatype>
bool quadtree<datatype>::insert( const size_t X, const size_t Y, const datatype & D ){
    if ( X < width && Y < height )
    {
        if ( root != nullptr && root->data != D )
        {
            const size_t temp = root->insert( X, Y, D );
            if ( temp != -1 )
            {
                count.quads += temp;
                ++count.points;
            }
        }
            // If the quadtree is empty.
        else if ( root == nullptr )
        {
            root = new quad( 0, 0, width, height, D );
            ++count.quads;
        }
        return true;
    }
    else return false;
}

template<class datatype>
datatype quadtree<datatype>::getData( const size_t X, const size_t Y ) const{
    return root->getData( X, Y );
}

template <class datatype>
void quadtree<datatype>::optimize(){
    if ( root != nullptr ) root->optimize( count.quads, count.points );
}

template <class datatype>
void quadtree<datatype>::clear()
{
    count = { 0, 0 };
    delete root;
    root = nullptr;
}

template <class datatype>
size_t quadtree<datatype>::numQuads()
{
    return count.quads;
}
template <class datatype>
size_t quadtree<datatype>::numPoints()
{
    return count.points;
}
template <class datatype>
size_t quadtree<datatype>::size()
{
    return ( sizeof( quadtree<datatype> ) ) +
           ( count.quads * sizeof( quadtree<datatype>::quad ) ) +
           ( count.points * sizeof( quadtree<datatype>::point ) );
}


#endif /* QUADTREE_H */