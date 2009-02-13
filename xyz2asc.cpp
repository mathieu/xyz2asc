// Mathieu MARACHE mathieu.marache@cstb.fr
#include <iostream>
#include <fstream>
#include <vector>
#include <climits>

#define STEP_SIZE 2
#define NO_DATA 0
long min_x, min_y = LONG_MAX;
long max_x, max_y = 0;
#define X_SIZE ( ( max_x - min_x ) / STEP_SIZE )
#define Y_SIZE ( ( max_y - min_y ) / STEP_SIZE )

// ROWS = Y
// COLUMNS = X

int xyz2asc(std::ostream& output, int argc, char *argv[]);
bool detectExtent(int argc, char *argv[]);

int main(int argc, char *argv[])
{

    if (argc==1) 
    {
        std::cerr << "usage : " << argv[0] << "[-o output_file] file.xyz [file.xyz]" 
                  << std::endl;
        return 1;
    }
    else  if (argc>3) 
    {
        if(argv[1][0]=='-' && argv[1][1]=='o' )
        {
            if (detectExtent(argc-2,&(argv[2])))
            {
                std::ofstream out(argv[2]);
                int ret = xyz2asc(out,argc-2,&(argv[2]));
                out.close();
                return ret;
            }
            else
            {
                std::cerr << "Abort : unable to detect extend" << std::endl;
                return 1;
            }
        }
    } 
    else
    {
        if (detectExtent(argc,argv))
        {
            return xyz2asc(std::cout,argc,argv);
        }
        else
        {
            std::cerr << "Abort : unable to detect extend" << std::endl;
            return 1;
        }
    }
}


bool detectExtent( int argc, char *argv[])
{
        for (int file=1;file<argc;++file)
    {
        std::ifstream input;
        input.open(argv[file]);

        if(input.is_open())
        {
            std::cerr << "calculating extent " << file << "/" << argc-1 <<" : " 
                      << argv[file] << std::endl;
            int n=0;
            float x,y,h;
            
            while(input.good())
            {
                input >> n >> x >> y >> h;
                if (x > max_x)
                {
                    max_x = x;
                }
                if (x < min_x)
                {
                    min_x = x;
                }
                if (y > max_y)
                {
                    max_y = y;
                }
                if (y < min_y)
                {
                    min_y = y;
                }
            }
            input.close();
        } 
        else
        {
            std::cerr << "Abort : unable to open " << argv[file] << std::endl;
            return false;
        }
    }

    return true;
}

int xyz2asc(std::ostream& out, int argc, char *argv[])
{
    // 1- build a grid of Y_SIZE x X_SIZE
    // 2- fill it with NO_DATA value
    // 3- parse files in argument list to fill in the array
    // 4- write the array to a file

    // 1-
    std::vector< std::vector<float> > heights;

    // 2-
    for (unsigned int y=0;y<Y_SIZE;++y)
    {
        std::vector<float> row;
        for (unsigned int x=0;x<X_SIZE;++x)
        {
            row.push_back(NO_DATA);
        }
        heights.push_back(row);
    }
   
    // 3

    for (int file=1;file<argc;++file)
    {
        std::ifstream input;
        input.open(argv[file]);

        if(input.is_open())
        {
            std::cerr << "parsing "<< file << "/" << argc-1 <<" : " 
                      << argv[file] << std::endl;
            int n=0;
            float x,y,h;
            
            while(input.good())
            {
                input >> n >> x >> y >> h;
#if 0
                std::cerr << ((int)y - min_y)/STEP_SIZE <<","
                          << ((int)x - min_x)/STEP_SIZE << std::endl;
#endif
                heights[((int)y - min_y)/STEP_SIZE]
                       [((int)x - min_x)/STEP_SIZE] = h;
            }
            input.close();
        } 
        else
        {
            std::cerr << "Abort : unable to open " << argv[file] << std::endl;
            return 2;
        }
    }

    // 4
    std::cerr << "writing output" << std::endl;
    out << "ncols        " << X_SIZE << std::endl;
    out << "nrows        " << Y_SIZE << std::endl;
    out << "xllcorner    " << min_x << std::endl;
    out << "yllcorner    " << min_y << std::endl;
    out << "cellsize     " << STEP_SIZE << std::endl;
    out << "NODATA_value " << NO_DATA << std::endl;

    std::vector< std::vector< float > >::reverse_iterator row = heights.rbegin();
    while(row != heights.rend())
    {
        std::vector<float>::iterator column = (*row).begin();
        while(column != (*row).end())
        {
            out << *column << " ";
            ++column;
        }
        out << std::endl;
        ++row;
    }

    return 0;
}

