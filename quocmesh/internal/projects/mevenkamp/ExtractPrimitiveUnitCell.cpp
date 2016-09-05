#include <aol.h>

#include <patternAnalysis.h>

typedef double RType;
typedef qc::ScalarArray<RType, qc::QC_2D> PicType;
typedef qc::MultiArray<RType, qc::QC_2D, 3> ColoredPicType;

typedef RType RealType;
typedef PicType PictureType;
typedef ColoredPicType ColoredPictureType;


int main( int argc, char **argv ) {
  try {
    aol::ParameterParser parser ( argc, argv, "unitCellExtraction.par" );

    const std::string srcPath = parser.getString ( "sourcePath" );
    const std::string outputDir = parser.getString ( "outputDir" );
    
    const int angleAlgTypeIdx = parser.getIntOrDefault ( "angleAlgType", 0 );
    im::PatternAnalysisLatticeAngleOptimizationType angleAlgType;
    if ( angleAlgTypeIdx == 1 ) angleAlgType = im::FourierPeaks;
    else if ( angleAlgTypeIdx == 2 ) angleAlgType = im::ProjectiveStdDevPeaks;
    
    const int periodAlgTypeIdx = parser.getIntOrDefault ( "periodAlgType", 0 );
    im::PatternAnalysisFundamentalPeriodOptimizationType periodAlgType;
    if ( periodAlgTypeIdx == 1 ) periodAlgType = im::SineFit;
    else if ( periodAlgTypeIdx == 2 ) periodAlgType = im::PeriodicityEnergyMinimization;

    aol::Matrix22<int> matrix;
    if ( parser.hasVariable ( "matrix" ) ) {
      aol::MultiVector<RealType> mVec;
      parser.getRealMultiVec ( "matrix", mVec );
      for ( short row=0; row<2 ; ++row )
        for ( short col=0; col<2 ; ++col)
          matrix.set ( row, col, mVec[row][col] );
    }

    bool drawMainAxes = parser.hasVariable ( "patternOrigin" );
    aol::Vec2<short> patternOrigin;
    aol::Vector<short> patternOriginTmp ( 2 );
    if ( drawMainAxes )
      parser.getRealVec ( "patternOrigin", patternOriginTmp );
    if ( patternOriginTmp.size ( ) == 2 )
      patternOrigin.set ( patternOriginTmp[0], patternOriginTmp[1] );

    std::string dirPath = aol::strprintf ( "%s/%s", outputDir.c_str ( ), aol::getBaseFileName ( srcPath ).c_str ( ) );
    aol::makeDirectory ( dirPath.c_str ( ) );

    PictureType data ( srcPath.c_str ( ) );
    data.save ( aol::strprintf ( "%s/data%s", dirPath.c_str ( ), qc::getDefaultArraySuffix ( qc::QC_2D ) ).c_str ( ), qc::PGM_DOUBLE_BINARY );

    aol::MultiVector<RealType> latticeVectors;
    im::PatternAnalyzer<RealType, PictureType> patternAnalyzer ( dirPath, parser.checkAndGetBool ( "quietMode" ) );
    
    aol::StopWatch watch;
    watch.start ( );
    
    patternAnalyzer.getLatticeVectors ( latticeVectors, data, angleAlgType, periodAlgType, parser.checkAndGetBool ( "refine" ), parser.getIntOrDefault ( "meanFilterWindowSize", 5 ) );
    
    latticeVectors.saveASCII ( aol::strprintf ( "%s/latticeVectors.txt", dirPath.c_str ( ) ).c_str ( ) );
    
    watch.stop ( );
    watch.printReport ( std::cerr );
  } catch ( aol::Exception &el ) {
    el.dump();
  }
  aol::callSystemPauseIfNecessaryOnPlatform();
  return 0;
}
