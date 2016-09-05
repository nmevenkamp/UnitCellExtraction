In order to COMPILE the Unit Cell Extraction code under UNIX, follow these steps:

1) In quocGCC/go.sh, specify path to compiler binaries (CC=... & CXX=...), e.g. GCC
2) execute go.sh
3) run make

In order to RUN the Unit Cell Extraction code, follow these steps:

1) In patternAnalysis.par, specify sourcePath (*.tif) and outputDirectory
2) Execute quocGCC/internal/projects/mevenkamp/ExtractPrimitiveUnitCell <pathTo unitCellExtraction.par>

The results will be stored in a folder inside <outputDirectory>.
The name of the folder is the base name of the input file.
The lattice vectors are saved to the file "latticeVectors.txt" in CSV format with " " separator.


Please note that the provided test images are in double precision (32 bit) TIFF format.
These should be compatible with many popular software packages, such as
Digital Micrograph, MATLAB, scipy.


In order to COMPILE and run under WINDOWS, create a batch file (.bat) similar to the go.sh
and e.g. use mingw32 as compiler.

You can circumvent building the TIFF library under Windows
by dropping the -DUSE_TIFF=1 from the cmake script
and loading the .q2bz files instead of the .tif ones.