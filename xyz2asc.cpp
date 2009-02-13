// Mathieu MARACHE mathieu.marache@cstb.fr
#include <iostream>
#include <fstream>
#include <vector>

#define STEP_SIZE 2
#define NO_DATA 0
#define MIN_X 1032501
#define MAX_X 1040001
#define MIN_Y 6291001
#define MAX_Y 6318001
#define X_SIZE ( ( MAX_X - MIN_X ) / STEP_SIZE )
#define Y_SIZE ( ( MAX_Y - MIN_Y ) / STEP_SIZE )

// ROWS = Y
// COLUMNS = X

int xyz2asc(std::ostream& output, int argc, char *argv[]);

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
            std::ofstream out(argv[2]);
            return xyz2asc(out,argc-2,&(argv[2]));
        }
    } 
    else
    {
        return xyz2asc(std::cout,argc,argv);
    }
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
            std::cerr << file << "/" << argc-1 <<" : " 
                      << argv[file] << std::endl;
            int n=0;
            float x,y,h;
            
            while(input.good())
            {
                input >> n >> x >> y >> h;
#if 0
                std::cerr << ((int)y - MIN_Y)/STEP_SIZE <<","
                          << ((int)x - MIN_X)/STEP_SIZE << std::endl;
#endif
                heights[((int)y - MIN_Y)/STEP_SIZE]
                       [((int)x - MIN_X)/STEP_SIZE] = h;
            }
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
    out << "xllcorner    " << MIN_X << std::endl;
    out << "yllcorner    " << MIN_Y << std::endl;
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

