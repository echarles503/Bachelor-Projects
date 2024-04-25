unsigned char *
GetColorMap(int &textureSize)
{
    unsigned char controlPts[8][3] =
    {
        {  71,  71, 219 },
        {   0,   0,  91 },
        {   0, 255, 255 },
        {   0, 127,   0 },
        { 255, 255,   0 },
        { 255,  96,   0 },
        { 107,   0,   0 },
        { 224,  76,  76 },
    };
    textureSize = 256;
    unsigned char *ptr = new unsigned char[textureSize*3];
    int nControlPts = 8;
    double amountPerPair = ((double)textureSize-1.0)/(nControlPts-1.0);
    for (int i = 0 ; i < textureSize ; i++)
    {
        int lowerControlPt = (int)(i/amountPerPair);
        int upperControlPt = lowerControlPt+1;
        if (upperControlPt >= nControlPts)
            upperControlPt = lowerControlPt; // happens for i == textureSize-1

        double proportion = (i/amountPerPair)-lowerControlPt;
        for (int j = 0 ; j < 3 ; j++)
            ptr[3*i+j] = controlPts[lowerControlPt][j]
                       + proportion*(controlPts[upperControlPt][j]-
                                     controlPts[lowerControlPt][j]);
    }

    return ptr;
}

unsigned char *
GetTigerStripes(int &textureSize)
{
    textureSize = 2048;
    unsigned char *ptr = new unsigned char[textureSize];
    int numStripes = 20;
    int valsPerStripe = textureSize/numStripes;
    for (int i = 0 ; i < numStripes ; i++)
    {
        for (int j = 0 ; j < valsPerStripe ; j++)
        {
           int idx = i*valsPerStripe+j;
           if (j < valsPerStripe / 3)
               ptr[idx] = 152;
           else
               ptr[idx] = 255;
        }
    }
    for (int i = numStripes*valsPerStripe ; i < textureSize ; i++)
    {
        ptr[i] = 0;
    }
    return ptr;
}
