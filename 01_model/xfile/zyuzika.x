xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 366;
 2.83130;22.04068;-1.02414;,
 2.83130;22.04068;1.02296;,
 2.83130;3.54447;0.81719;,
 2.83130;3.54447;-0.81837;,
 -2.83130;22.04068;1.02296;,
 -2.83130;22.04068;-1.02414;,
 -2.83130;3.54447;-0.81837;,
 -2.83130;3.54447;0.81719;,
 1.41565;1.87703;0.81719;,
 1.41565;1.87703;-0.81837;,
 2.83130;3.54447;-0.81837;,
 1.41565;1.87703;0.81719;,
 2.83130;3.54447;0.81719;,
 2.83130;22.04068;1.02296;,
 1.41565;25.84975;1.02296;,
 0.00000;25.84975;1.02296;,
 0.00000;1.87703;0.81719;,
 1.41565;1.87703;-0.81837;,
 0.00000;1.87703;-0.81837;,
 0.00000;25.84975;-1.02414;,
 1.41565;25.84975;-1.02414;,
 2.83130;22.04068;-1.02414;,
 2.83130;3.54447;-0.81837;,
 1.41565;25.84975;1.02296;,
 2.83130;22.04068;-1.02414;,
 1.41565;25.84975;-1.02414;,
 0.00000;25.84975;-1.02414;,
 -1.41565;1.87703;0.81719;,
 -2.83130;3.54447;-0.81837;,
 -1.41565;1.87703;-0.81837;,
 -1.41565;1.87703;0.81719;,
 -1.41565;25.84975;1.02296;,
 -2.83130;22.04068;1.02296;,
 -2.83130;3.54447;0.81719;,
 -1.41565;1.87703;-0.81837;,
 -2.83130;3.54447;-0.81837;,
 -2.83130;22.04068;-1.02414;,
 -1.41565;25.84975;-1.02414;,
 -1.41565;25.84975;1.02296;,
 -1.41565;25.84975;-1.02414;,
 -2.83130;22.04068;-1.02414;,
 0.00000;0.32208;0.54540;,
 1.41565;0.67000;0.54540;,
 1.41565;0.67000;-0.54658;,
 0.00000;0.32208;-0.54658;,
 0.00000;1.87703;-0.81837;,
 -1.41565;0.67000;-0.54658;,
 -1.41565;0.67000;0.54540;,
 7.67712;14.56512;-1.20126;,
 7.67712;14.56512;1.20008;,
 -7.67712;14.56512;1.20008;,
 -7.67712;14.56512;-1.20126;,
 7.45077;18.65981;1.20008;,
 7.45077;18.65981;-1.20126;,
 -7.45077;18.65981;-1.20126;,
 -7.45077;18.65981;1.20008;,
 9.95023;16.61247;-1.20126;,
 7.45077;18.65981;-1.20126;,
 9.95023;16.61247;1.20008;,
 7.67712;14.56512;-1.20126;,
 9.95023;16.61247;-1.20126;,
 7.67712;14.56512;-1.20126;,
 -7.67712;14.56512;-1.20126;,
 -9.95023;16.61247;-1.20126;,
 -7.45077;18.65981;-1.20126;,
 7.45077;18.65981;-1.20126;,
 7.45077;18.65981;1.20008;,
 -7.45077;18.65981;1.20008;,
 -9.95023;16.61247;1.20008;,
 -7.67712;14.56512;1.20008;,
 7.67712;14.56512;1.20008;,
 -9.95023;16.61247;-1.20126;,
 -7.67712;14.56512;-1.20126;,
 -7.45077;18.65981;-1.20126;,
 8.22931;17.20526;-1.23773;,
 8.22931;15.86552;-1.23773;,
 -8.04726;15.86552;-1.23773;,
 -8.04726;17.20526;-1.23773;,
 8.22931;15.86552;-1.23773;,
 8.22931;15.86552;1.23655;,
 -8.04726;15.86552;1.23655;,
 -8.04726;15.86552;-1.23773;,
 8.22931;15.86552;1.23655;,
 8.22931;17.20526;1.23655;,
 -8.04726;17.20526;1.23655;,
 -8.04726;15.86552;1.23655;,
 8.22931;17.20526;1.23655;,
 8.22931;17.20526;-1.23773;,
 -8.04726;17.20526;-1.23773;,
 -8.04726;17.20526;1.23655;,
 8.22931;15.86552;-1.23773;,
 8.22931;17.20526;-1.23773;,
 -8.04726;17.20526;-1.23773;,
 -8.04726;15.86552;-1.23773;,
 6.57648;15.38796;-1.23773;,
 6.57648;14.96465;-1.23773;,
 -6.53414;14.96465;-1.23773;,
 -6.53414;15.38796;-1.23773;,
 6.57648;14.96465;-1.23773;,
 6.57648;14.96465;1.23655;,
 -6.53414;14.96465;1.23655;,
 -6.53414;14.96465;-1.23773;,
 6.57648;14.96465;1.23655;,
 6.57648;15.38796;1.23655;,
 -6.53414;15.38796;1.23655;,
 -6.53414;14.96465;1.23655;,
 6.57648;15.38796;1.23655;,
 6.57648;15.38796;-1.23773;,
 -6.53414;15.38796;-1.23773;,
 -6.53414;15.38796;1.23655;,
 6.57648;14.96465;-1.23773;,
 6.57648;15.38796;-1.23773;,
 -6.53414;15.38796;-1.23773;,
 -6.53414;14.96465;-1.23773;,
 6.60925;18.12757;-1.23773;,
 6.60925;17.70425;-1.23773;,
 -6.56690;17.70425;-1.23773;,
 -6.56690;18.12757;-1.23773;,
 6.60925;17.70425;-1.23773;,
 6.60925;17.70425;1.23655;,
 -6.56690;17.70425;1.23655;,
 -6.56690;17.70425;-1.23773;,
 6.60925;17.70425;1.23655;,
 6.60925;18.12757;1.23655;,
 -6.56690;18.12757;1.23655;,
 -6.56690;17.70425;1.23655;,
 6.60925;18.12757;1.23655;,
 6.60925;18.12757;-1.23773;,
 -6.56690;18.12757;-1.23773;,
 -6.56690;18.12757;1.23655;,
 6.60925;17.70425;-1.23773;,
 6.60925;18.12757;-1.23773;,
 -6.56690;18.12757;-1.23773;,
 -6.56690;17.70425;-1.23773;,
 0.00000;15.75247;-1.32939;,
 0.50289;15.96078;-1.32939;,
 0.50289;15.96078;1.32704;,
 0.00000;15.75247;1.32704;,
 0.71119;16.46367;-1.32939;,
 0.71119;16.46367;1.32704;,
 0.50289;16.96656;-1.32939;,
 0.50289;16.96656;1.32704;,
 -0.00000;17.17487;-1.32939;,
 -0.00000;17.17487;1.32704;,
 -0.50289;16.96656;-1.32939;,
 -0.50289;16.96656;1.32704;,
 -0.71119;16.46367;-1.32939;,
 -0.71119;16.46367;1.32704;,
 -0.50289;15.96078;-1.32939;,
 -0.50289;15.96078;1.32704;,
 0.00000;15.75247;-1.32939;,
 0.00000;15.75247;1.32704;,
 0.00000;16.46367;-1.32939;,
 0.00000;16.46367;-1.32939;,
 0.00000;16.46367;-1.32939;,
 0.00000;16.46367;-1.32939;,
 0.00000;16.46367;-1.32939;,
 0.00000;16.46367;-1.32939;,
 0.00000;16.46367;-1.32939;,
 0.00000;16.46367;-1.32939;,
 0.00000;16.46367;1.32704;,
 0.00000;16.46367;1.32704;,
 0.00000;16.46367;1.32704;,
 0.00000;16.46367;1.32704;,
 0.00000;16.46367;1.32704;,
 0.00000;16.46367;1.32704;,
 0.00000;16.46367;1.32704;,
 0.00000;16.46367;1.32704;,
 2.42571;21.82014;-1.20126;,
 2.42571;21.82014;1.20008;,
 2.42571;3.77947;0.95870;,
 2.42571;3.77947;-0.95988;,
 -2.42571;21.82014;1.20008;,
 -2.42571;21.82014;-1.20126;,
 -2.42571;3.77947;-0.95988;,
 -2.42571;3.77947;0.95870;,
 1.21285;2.15309;0.95870;,
 1.21285;2.15309;-0.95988;,
 2.42571;3.77947;-0.95988;,
 1.21285;2.15309;0.95870;,
 2.42571;3.77947;0.95870;,
 2.42571;21.82014;1.20008;,
 1.21285;25.53539;1.20008;,
 0.00000;25.53539;1.20008;,
 0.00000;2.15309;0.95870;,
 1.21285;2.15309;-0.95988;,
 0.00000;2.15309;-0.95988;,
 0.00000;25.53539;-1.20126;,
 1.21285;25.53539;-1.20126;,
 2.42571;21.82014;-1.20126;,
 2.42571;3.77947;-0.95988;,
 1.21285;25.53539;1.20008;,
 2.42571;21.82014;-1.20126;,
 1.21285;25.53539;-1.20126;,
 0.00000;25.53539;-1.20126;,
 -1.21285;2.15309;0.95870;,
 -2.42571;3.77947;-0.95988;,
 -1.21285;2.15309;-0.95988;,
 -1.21285;2.15309;0.95870;,
 -1.21285;25.53539;1.20008;,
 -2.42571;21.82014;1.20008;,
 -2.42571;3.77947;0.95870;,
 -1.21285;2.15309;-0.95988;,
 -2.42571;3.77947;-0.95988;,
 -2.42571;21.82014;-1.20126;,
 -1.21285;25.53539;-1.20126;,
 -1.21285;25.53539;1.20008;,
 -1.21285;25.53539;-1.20126;,
 -2.42571;21.82014;-1.20126;,
 0.00000;0.63645;0.63988;,
 1.21285;0.97579;0.63988;,
 1.21285;0.97579;-0.64106;,
 0.00000;0.63645;-0.64106;,
 0.00000;2.15309;-0.95988;,
 -1.21285;0.97579;-0.64106;,
 -1.21285;0.97579;0.63988;,
 -9.95023;16.61247;-1.20126;,
 -7.67712;14.56512;-1.20126;,
 -7.67712;14.56512;1.20008;,
 -9.95023;16.61247;1.20008;,
 -7.45077;18.65981;1.20008;,
 -7.45077;18.65981;-1.20126;,
 -0.60931;24.91126;-1.33384;,
 0.60931;24.91126;-1.33384;,
 0.60931;2.77723;-1.06580;,
 -0.60931;2.77723;-1.06580;,
 0.60931;24.91126;-1.33384;,
 0.60931;24.91126;1.33265;,
 0.60931;2.77723;1.06462;,
 0.60931;2.77723;-1.06580;,
 0.60931;24.91126;1.33265;,
 -0.60931;24.91126;1.33265;,
 -0.60931;2.77723;1.06462;,
 0.60931;2.77723;1.06462;,
 -0.60931;24.91126;1.33265;,
 -0.60931;24.91126;-1.33384;,
 -0.60931;2.77723;-1.06580;,
 -0.60931;2.77723;1.06462;,
 0.60931;24.91126;-1.33384;,
 -0.60931;24.91126;-1.33384;,
 -0.60931;2.77723;-1.06580;,
 0.60931;2.77723;-1.06580;,
 1.06004;23.42091;-1.34720;,
 1.48335;23.42091;-1.34720;,
 1.48335;4.26758;-1.07648;,
 1.06004;4.26758;-1.07648;,
 1.48335;23.42091;-1.34720;,
 1.48335;23.42091;1.34602;,
 1.48335;4.26758;1.07530;,
 1.48335;4.26758;-1.07648;,
 1.48335;23.42091;1.34602;,
 1.06004;23.42091;1.34602;,
 1.06004;4.26758;1.07530;,
 1.48335;4.26758;1.07530;,
 1.06004;23.42091;1.34602;,
 1.06004;23.42091;-1.34720;,
 1.06004;4.26758;-1.07648;,
 1.06004;4.26758;1.07530;,
 1.48335;23.42091;-1.34720;,
 1.06004;23.42091;-1.34720;,
 1.06004;4.26758;-1.07648;,
 1.48335;4.26758;-1.07648;,
 -1.55901;23.42091;-1.38121;,
 -1.13569;23.42091;-1.38121;,
 -1.13569;4.26758;-1.10365;,
 -1.55901;4.26758;-1.10365;,
 -1.13569;23.42091;-1.38121;,
 -1.13569;23.42091;1.38002;,
 -1.13569;4.26758;1.10247;,
 -1.13569;4.26758;-1.10365;,
 -1.13569;23.42091;1.38002;,
 -1.55901;23.42091;1.38002;,
 -1.55901;4.26758;1.10247;,
 -1.13569;4.26758;1.10247;,
 -1.55901;23.42091;1.38002;,
 -1.55901;23.42091;-1.38121;,
 -1.55901;4.26758;-1.10365;,
 -1.55901;4.26758;1.10247;,
 -1.13569;23.42091;-1.38121;,
 -1.55901;23.42091;-1.38121;,
 -1.55901;4.26758;-1.10365;,
 -1.13569;4.26758;-1.10365;,
 -8.04726;17.20526;-1.23773;,
 -8.04726;15.86552;-1.23773;,
 -8.04726;15.86552;1.23655;,
 -8.04726;17.20526;1.23655;,
 6.57648;15.38796;-1.23773;,
 6.57648;14.96465;-1.23773;,
 -6.53414;14.96465;-1.23773;,
 -6.53414;15.38796;-1.23773;,
 6.57648;14.96465;-1.23773;,
 6.57648;14.96465;1.23655;,
 -6.53414;14.96465;1.23655;,
 -6.53414;14.96465;-1.23773;,
 6.57648;14.96465;1.23655;,
 6.57648;15.38796;1.23655;,
 -6.53414;15.38796;1.23655;,
 -6.53414;14.96465;1.23655;,
 6.57648;15.38796;1.23655;,
 6.57648;15.38796;-1.23773;,
 -6.53414;15.38796;-1.23773;,
 -6.53414;15.38796;1.23655;,
 6.57648;14.96465;-1.23773;,
 6.57648;15.38796;-1.23773;,
 -6.53414;15.38796;-1.23773;,
 -6.53414;14.96465;-1.23773;,
 0.00000;15.75247;-1.32939;,
 0.50289;15.96078;-1.32939;,
 0.50289;15.96078;1.32704;,
 0.00000;15.75247;1.32704;,
 0.71119;16.46367;-1.32939;,
 0.71119;16.46367;1.32704;,
 0.50289;16.96656;-1.32939;,
 0.50289;16.96656;1.32704;,
 -0.00000;17.17487;-1.32939;,
 -0.00000;17.17487;1.32704;,
 -0.50289;16.96656;-1.32939;,
 -0.50289;16.96656;1.32704;,
 -0.71119;16.46367;-1.32939;,
 -0.71119;16.46367;1.32704;,
 -0.50289;15.96078;-1.32939;,
 -0.50289;15.96078;1.32704;,
 0.00000;15.75247;-1.32939;,
 0.00000;15.75247;1.32704;,
 0.00000;16.46367;-1.32939;,
 0.00000;16.46367;-1.32939;,
 0.00000;16.46367;-1.32939;,
 0.00000;16.46367;-1.32939;,
 0.00000;16.46367;-1.32939;,
 0.00000;16.46367;-1.32939;,
 0.00000;16.46367;-1.32939;,
 0.00000;16.46367;-1.32939;,
 0.00000;16.46367;1.32704;,
 0.00000;16.46367;1.32704;,
 0.00000;16.46367;1.32704;,
 0.00000;16.46367;1.32704;,
 0.00000;16.46367;1.32704;,
 0.00000;16.46367;1.32704;,
 0.00000;16.46367;1.32704;,
 0.00000;16.46367;1.32704;,
 8.06973;14.26998;-1.07113;,
 8.06973;14.26998;1.06995;,
 -8.06973;14.26998;1.06995;,
 -8.06973;14.26998;-1.07113;,
 7.83181;18.95495;1.06995;,
 7.83181;18.95495;-1.07113;,
 -7.83180;18.95495;-1.07113;,
 -7.83180;18.95495;1.06995;,
 10.45909;16.61247;-1.07113;,
 7.83181;18.95495;-1.07113;,
 10.45909;16.61247;1.06995;,
 8.06973;14.26998;-1.07113;,
 10.45909;16.61247;-1.07113;,
 8.06973;14.26998;-1.07113;,
 -8.06973;14.26998;-1.07113;,
 -10.45909;16.61247;-1.07113;,
 -7.83180;18.95495;-1.07113;,
 7.83181;18.95495;-1.07113;,
 7.83181;18.95495;1.06995;,
 -7.83180;18.95495;1.06995;,
 -10.45909;16.61247;1.06995;,
 -8.06973;14.26998;1.06995;,
 8.06973;14.26998;1.06995;,
 -10.45909;16.61247;-1.07113;,
 -8.06973;14.26998;-1.07113;,
 -7.83180;18.95495;-1.07113;;
 
 161;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,2;,
 4;11,12,13,14;,
 4;11,14,15,16;,
 4;17,18,19,20;,
 4;17,20,21,22;,
 4;23,1,24,25;,
 4;23,25,26,15;,
 4;27,7,28,29;,
 4;30,16,15,31;,
 4;30,31,32,33;,
 4;34,35,36,37;,
 4;34,37,19,18;,
 4;38,15,26,39;,
 4;38,39,40,4;,
 4;41,42,8,16;,
 4;43,44,45,9;,
 4;42,43,9,8;,
 4;42,41,44,43;,
 4;46,47,27,29;,
 4;44,46,29,45;,
 4;47,41,16,27;,
 4;47,46,44,41;,
 4;48,49,50,51;,
 4;52,53,54,55;,
 4;56,57,52,58;,
 4;56,58,49,59;,
 4;60,61,62,63;,
 4;60,63,64,65;,
 4;58,66,67,68;,
 4;58,68,69,70;,
 4;71,72,50,68;,
 4;71,68,55,73;,
 4;74,75,76,77;,
 4;78,79,80,81;,
 4;82,83,84,85;,
 4;86,87,88,89;,
 4;86,79,90,91;,
 4;92,93,80,89;,
 4;94,95,96,97;,
 4;98,99,100,101;,
 4;102,103,104,105;,
 4;106,107,108,109;,
 4;106,99,110,111;,
 4;112,113,100,109;,
 4;114,115,116,117;,
 4;118,119,120,121;,
 4;122,123,124,125;,
 4;126,127,128,129;,
 4;126,119,130,131;,
 4;132,133,120,129;,
 4;134,135,136,137;,
 4;135,138,139,136;,
 4;138,140,141,139;,
 4;140,142,143,141;,
 4;142,144,145,143;,
 4;144,146,147,145;,
 4;146,148,149,147;,
 4;148,150,151,149;,
 3;152,135,134;,
 3;153,138,135;,
 3;154,140,138;,
 3;155,142,140;,
 3;156,144,142;,
 3;157,146,144;,
 3;158,148,146;,
 3;159,150,148;,
 3;160,137,136;,
 3;161,136,139;,
 3;162,139,141;,
 3;163,141,143;,
 3;164,143,145;,
 3;165,145,147;,
 3;166,147,149;,
 3;167,149,151;,
 4;168,169,170,171;,
 4;172,173,174,175;,
 4;176,177,178,170;,
 4;179,180,181,182;,
 4;179,182,183,184;,
 4;185,186,187,188;,
 4;185,188,189,190;,
 4;191,169,192,193;,
 4;191,193,194,183;,
 4;195,175,196,197;,
 4;198,184,183,199;,
 4;198,199,200,201;,
 4;202,203,204,205;,
 4;202,205,187,186;,
 4;206,183,194,207;,
 4;206,207,208,172;,
 4;209,210,176,184;,
 4;211,212,213,177;,
 4;210,211,177,176;,
 4;210,209,212,211;,
 4;214,215,195,197;,
 4;212,214,197,213;,
 4;215,209,184,195;,
 4;215,214,212,209;,
 4;216,217,218,219;,
 4;216,219,220,221;,
 4;222,223,224,225;,
 4;226,227,228,229;,
 4;230,231,232,233;,
 4;234,235,236,237;,
 4;234,227,238,239;,
 4;240,241,228,237;,
 4;242,243,244,245;,
 4;246,247,248,249;,
 4;250,251,252,253;,
 4;254,255,256,257;,
 4;254,247,258,259;,
 4;260,261,248,257;,
 4;262,263,264,265;,
 4;266,267,268,269;,
 4;270,271,272,273;,
 4;274,275,276,277;,
 4;274,267,278,279;,
 4;280,281,268,277;,
 4;282,283,284,285;,
 4;286,287,288,289;,
 4;290,291,292,293;,
 4;294,295,296,297;,
 4;298,299,300,301;,
 4;298,291,302,303;,
 4;304,305,292,301;,
 4;306,307,308,309;,
 4;307,310,311,308;,
 4;310,312,313,311;,
 4;312,314,315,313;,
 4;314,316,317,315;,
 4;316,318,319,317;,
 4;318,320,321,319;,
 4;320,322,323,321;,
 3;324,307,306;,
 3;325,310,307;,
 3;326,312,310;,
 3;327,314,312;,
 3;328,316,314;,
 3;329,318,316;,
 3;330,320,318;,
 3;331,322,320;,
 3;332,309,308;,
 3;333,308,311;,
 3;334,311,313;,
 3;335,313,315;,
 3;336,315,317;,
 3;337,317,319;,
 3;338,319,321;,
 3;339,321,323;,
 4;340,341,342,343;,
 4;344,345,346,347;,
 4;348,349,344,350;,
 4;348,350,341,351;,
 4;352,353,354,355;,
 4;352,355,356,357;,
 4;350,358,359,360;,
 4;350,360,361,362;,
 4;363,364,342,360;,
 4;363,360,347,365;;
 
 MeshMaterialList {
  4;
  161;
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3;;
  Material {
   0.000000;0.015686;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.746667;0.800000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  136;
  -0.984215;0.176979;0.000000;,
  -0.938701;-0.344733;0.000000;,
  0.984215;0.176979;0.000000;,
  0.938701;-0.344733;-0.000000;,
  0.000000;-0.102735;0.994709;,
  0.000000;-0.102735;-0.994709;,
  0.000000;-0.008583;0.999963;,
  0.000000;-0.008583;-0.999963;,
  -0.002497;-0.009219;0.999954;,
  -0.002497;-0.009219;-0.999954;,
  0.002497;-0.009219;0.999954;,
  0.002497;-0.009219;-0.999954;,
  0.000000;-0.195993;0.980605;,
  0.000000;-0.195993;-0.980605;,
  0.021150;-0.195949;0.980386;,
  0.021150;-0.195950;-0.980386;,
  -1.000000;0.000000;0.000000;,
  0.005407;-0.071748;0.997408;,
  -0.004994;-0.009854;0.999939;,
  0.005407;-0.071748;-0.997408;,
  -0.004994;-0.009854;-0.999939;,
  0.937357;0.348371;0.000000;,
  0.000000;1.000000;0.000000;,
  -0.005407;-0.071748;0.997408;,
  0.004994;-0.009854;0.999939;,
  -0.005407;-0.071748;-0.997408;,
  0.004994;-0.009854;-0.999939;,
  -0.937357;0.348371;0.000000;,
  1.000000;0.000000;0.000000;,
  0.238662;-0.971103;-0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.021150;-0.195950;-0.980386;,
  -0.021150;-0.195949;0.980386;,
  -0.238662;-0.971103;0.000000;,
  0.336450;0.941701;0.000000;,
  -0.336450;0.941701;0.000000;,
  0.358439;-0.933553;-0.000000;,
  -0.358439;-0.933553;0.000000;,
  0.633671;0.773603;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;1.000000;,
  0.669244;-0.743043;-0.000000;,
  -0.669243;-0.743043;0.000000;,
  -0.633670;0.773603;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;1.000000;0.000000;,
  1.000000;0.000002;0.000000;,
  -1.000000;-0.000001;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;1.000000;0.000000;,
  1.000000;0.000004;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;1.000000;0.000000;,
  1.000000;0.000005;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;-1.000000;-0.000000;,
  0.707112;-0.707102;-0.000000;,
  1.000000;0.000001;0.000000;,
  0.707111;0.707102;0.000000;,
  -0.000000;1.000000;0.000000;,
  -0.707111;0.707102;0.000000;,
  -1.000000;0.000001;0.000000;,
  -0.707112;-0.707102;0.000000;,
  0.000000;0.000000;1.000000;,
  -0.987580;0.157119;0.000000;,
  -0.949115;-0.314931;0.000000;,
  0.987580;0.157119;0.000000;,
  0.949115;-0.314931;-0.000000;,
  0.000000;-0.122755;0.992437;,
  0.000000;-0.122755;-0.992437;,
  0.000000;-0.010323;0.999947;,
  0.000000;-0.010323;-0.999947;,
  -0.003418;-0.011087;0.999933;,
  -0.003418;-0.011087;-0.999933;,
  0.003418;-0.011087;0.999933;,
  0.003418;-0.011087;-0.999933;,
  0.000000;-0.233674;0.972315;,
  0.000000;-0.233674;-0.972315;,
  0.028755;-0.233577;0.971913;,
  0.028755;-0.233577;-0.971913;,
  -1.000000;0.000000;0.000000;,
  -0.633670;0.773603;0.000000;,
  -0.669243;-0.743043;0.000000;,
  0.000000;-0.012109;-0.999927;,
  1.000000;0.000000;0.000000;,
  0.000000;-0.012109;0.999927;,
  0.000000;-0.014133;-0.999900;,
  0.000000;-0.014133;0.999900;,
  0.000000;-0.014490;-0.999895;,
  0.000000;-0.014490;0.999895;,
  -1.000000;-0.000001;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;1.000000;,
  0.707112;-0.707102;-0.000000;,
  1.000000;0.000001;0.000000;,
  0.707111;0.707102;0.000000;,
  -0.000000;1.000000;0.000000;,
  -0.707111;0.707102;0.000000;,
  -1.000000;0.000001;0.000000;,
  -0.707112;-0.707102;0.000000;,
  0.007348;-0.085741;0.996290;,
  -0.006836;-0.011851;0.999906;,
  0.007348;-0.085741;-0.996290;,
  -0.006836;-0.011851;-0.999906;,
  0.950627;0.310335;0.000000;,
  0.000000;1.000000;0.000000;,
  -0.007348;-0.085741;0.996290;,
  0.006836;-0.011851;0.999906;,
  -0.007348;-0.085741;-0.996290;,
  0.006836;-0.011851;-0.999906;,
  -0.950627;0.310335;0.000000;,
  0.269443;-0.963016;-0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.028755;-0.233577;-0.971913;,
  -0.028755;-0.233577;0.971913;,
  -0.269443;-0.963016;0.000000;,
  1.000000;0.000004;0.000000;,
  0.356087;0.934453;0.000000;,
  -0.356087;0.934453;0.000000;,
  0.378102;-0.925764;-0.000000;,
  -0.378102;-0.925764;0.000000;,
  0.665493;0.746404;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;1.000000;,
  0.700067;-0.714078;-0.000000;,
  -0.700066;-0.714078;0.000000;,
  -0.665492;0.746405;0.000000;;
  161;
  4;2,2,3,3;,
  4;0,0,1,1;,
  4;3,3,3,3;,
  4;17,18,18,8;,
  4;17,8,6,4;,
  4;19,5,7,9;,
  4;19,9,20,20;,
  4;21,2,2,21;,
  4;22,22,22,22;,
  4;1,1,1,1;,
  4;23,4,6,10;,
  4;23,10,24,24;,
  4;25,26,26,11;,
  4;25,11,7,5;,
  4;22,22,22,22;,
  4;27,27,0,0;,
  4;12,14,17,4;,
  4;15,13,5,19;,
  4;28,28,3,3;,
  4;29,30,30,29;,
  4;16,16,1,1;,
  4;13,31,25,5;,
  4;32,12,4,23;,
  4;33,33,30,30;,
  4;36,36,37,37;,
  4;34,34,35,35;,
  4;38,34,34,38;,
  4;41,41,36,36;,
  4;39,39,39,39;,
  4;39,39,39,39;,
  4;40,40,40,40;,
  4;40,40,40,40;,
  4;42,37,37,42;,
  4;43,43,35,35;,
  4;44,44,44,44;,
  4;45,45,45,45;,
  4;46,46,46,46;,
  4;47,47,47,47;,
  4;48,48,48,48;,
  4;49,49,49,49;,
  4;50,50,50,50;,
  4;51,51,51,51;,
  4;52,52,52,52;,
  4;53,53,53,53;,
  4;54,54,54,54;,
  4;55,55,55,55;,
  4;56,56,56,56;,
  4;57,57,57,57;,
  4;58,58,58,58;,
  4;59,59,59,59;,
  4;60,60,60,60;,
  4;61,61,61,61;,
  4;63,64,64,63;,
  4;64,65,65,64;,
  4;65,66,66,65;,
  4;66,67,67,66;,
  4;67,68,68,67;,
  4;68,69,69,68;,
  4;69,70,70,69;,
  4;70,63,63,70;,
  3;62,62,62;,
  3;62,62,62;,
  3;62,62,62;,
  3;62,62,62;,
  3;62,62,62;,
  3;62,62,62;,
  3;62,62,62;,
  3;62,62,62;,
  3;71,71,71;,
  3;71,71,71;,
  3;71,71,71;,
  3;71,71,71;,
  3;71,71,71;,
  3;71,71,71;,
  3;71,71,71;,
  3;71,71,71;,
  4;74,74,75,75;,
  4;72,72,73,73;,
  4;75,75,75,75;,
  4;109,110,110,80;,
  4;109,80,78,76;,
  4;111,77,79,81;,
  4;111,81,112,112;,
  4;113,74,74,113;,
  4;114,114,114,114;,
  4;73,73,73,73;,
  4;115,76,78,82;,
  4;115,82,116,116;,
  4;117,118,118,83;,
  4;117,83,79,77;,
  4;114,114,114,114;,
  4;119,119,72,72;,
  4;84,86,109,76;,
  4;87,85,77,111;,
  4;92,92,75,75;,
  4;120,121,121,120;,
  4;88,88,73,73;,
  4;85,122,117,77;,
  4;123,84,76,115;,
  4;124,124,121,121;,
  4;90,90,90,90;,
  4;89,89,89,89;,
  4;91,91,91,91;,
  4;92,92,92,92;,
  4;93,93,93,93;,
  4;88,88,88,88;,
  4;114,114,114,114;,
  4;100,100,100,100;,
  4;94,94,94,94;,
  4;92,92,92,92;,
  4;95,95,95,95;,
  4;88,88,88,88;,
  4;114,114,114,114;,
  4;100,100,100,100;,
  4;96,96,96,96;,
  4;92,92,92,92;,
  4;97,97,97,97;,
  4;88,88,88,88;,
  4;114,114,114,114;,
  4;100,100,100,100;,
  4;98,98,98,98;,
  4;99,99,99,99;,
  4;100,100,100,100;,
  4;101,101,101,101;,
  4;114,114,114,114;,
  4;125,125,125,125;,
  4;88,88,88,88;,
  4;100,102,102,100;,
  4;102,103,103,102;,
  4;103,104,104,103;,
  4;104,105,105,104;,
  4;105,106,106,105;,
  4;106,107,107,106;,
  4;107,108,108,107;,
  4;108,100,100,108;,
  3;99,99,99;,
  3;99,99,99;,
  3;99,99,99;,
  3;99,99,99;,
  3;99,99,99;,
  3;99,99,99;,
  3;99,99,99;,
  3;99,99,99;,
  3;101,101,101;,
  3;101,101,101;,
  3;101,101,101;,
  3;101,101,101;,
  3;101,101,101;,
  3;101,101,101;,
  3;101,101,101;,
  3;101,101,101;,
  4;128,128,129,129;,
  4;126,126,127,127;,
  4;130,126,126,130;,
  4;133,133,128,128;,
  4;131,131,131,131;,
  4;131,131,131,131;,
  4;132,132,132,132;,
  4;132,132,132,132;,
  4;134,129,129,134;,
  4;135,135,127,127;;
 }
 MeshTextureCoords {
  366;
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.750000;1.000000;,
  0.750000;0.000000;,
  1.000000;0.000000;,
  0.250000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.750000;1.000000;,
  0.500000;1.000000;,
  0.500000;0.000000;,
  0.750000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.750000;0.000000;,
  1.000000;1.000000;,
  0.750000;1.000000;,
  0.500000;1.000000;,
  0.250000;1.000000;,
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.750000;1.000000;,
  0.750000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.250000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.750000;1.000000;,
  0.750000;0.000000;,
  0.500000;0.000000;,
  0.500000;0.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.750000;1.000000;,
  0.750000;0.000000;,
  1.000000;0.000000;,
  0.250000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.750000;1.000000;,
  0.500000;1.000000;,
  0.500000;0.000000;,
  0.750000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.750000;0.000000;,
  1.000000;1.000000;,
  0.750000;1.000000;,
  0.500000;1.000000;,
  0.250000;1.000000;,
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.750000;1.000000;,
  0.750000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.250000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.750000;1.000000;,
  0.750000;0.000000;,
  0.500000;0.000000;,
  0.500000;0.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;;
 }
}