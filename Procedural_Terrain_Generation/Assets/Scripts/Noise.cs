using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UIElements;

public static class PerlinNoise
{
    public static float[,] GenerateNoiseMap(int mW, int mH, int sd, float sc, int oct, float pers, float lacu, Vector2 off)
    {
        System.Random rnd = new System.Random(sd);
        Vector2[] octOffs = new Vector2[oct];
        for (int i = 0; i < oct; i++)
        {
            float offX = rnd.Next(-100000, 100000) + off.x;
            float offY = rnd.Next(-100000, 100000) + off.y;
            octOffs[i] = new Vector2(offX, offY);
        }
        float [,] nMap = new float[mW, mH];

        if (sc <= 0)
            sc = 0.0001f;

        float maxNH = float.MinValue;
        float minNH = float.MaxValue;
        
        float halfW = mW / 2f;
        float halfH = mH / 2f;
        
        for (int y = 0; y < mH; y++)
        {
            for (int x = 0; x < mW; x++)
            {
                float amp = 1;
                float freq = 1;
                float nH = 0;
                for (int i = 0; i < oct; i++)
                {
                    float sX = (x-halfW) / sc * freq + octOffs[i].x;
                    float sY = (y- halfH) / sc * freq + octOffs[i].y;

                    float pVal = Mathf.PerlinNoise(sX, sY) * 2 - 1;
                    nH += pVal * amp;
                    
                    amp *= pers;
                    freq *= lacu;
                }
                if (nH > maxNH)
                    maxNH = nH;
                else if (nH < minNH)
                    minNH = nH;
                    
                nMap[x, y] = nH;
            }
        }

        for (int y = 0; y < mH; y++)
        {
            for (int x = 0; x < mW; x++)
            {
                nMap[x, y] = Mathf.InverseLerp(minNH, maxNH, nMap[x, y]);
            }
        }

        return nMap;
    }

}