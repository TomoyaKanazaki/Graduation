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
 10.45917;3.74099;33.07900;,
 10.45917;-3.82119;33.07900;,
 10.45917;-3.06105;-35.24775;,
 10.45917;2.98086;-35.24775;,
 -10.45900;-3.82119;33.07900;,
 -10.45900;3.74099;33.07900;,
 -10.45900;2.98086;-35.24775;,
 -10.45900;-3.06105;-35.24775;,
 5.22963;-3.06105;-41.40744;,
 5.22963;2.98086;-41.40744;,
 10.45917;2.98086;-35.24775;,
 5.22963;-3.06105;-41.40744;,
 10.45917;-3.06105;-35.24775;,
 10.45917;-3.82119;33.07900;,
 5.22963;-3.82119;47.15005;,
 0.00008;-3.82119;47.15005;,
 0.00008;-3.06105;-41.40744;,
 5.22963;2.98086;-41.40744;,
 0.00008;2.98086;-41.40744;,
 0.00008;3.74099;47.15005;,
 5.22963;3.74099;47.15005;,
 10.45917;3.74099;33.07900;,
 10.45917;2.98086;-35.24775;,
 5.22963;-3.82119;47.15005;,
 10.45917;3.74099;33.07900;,
 5.22963;3.74099;47.15005;,
 0.00008;3.74099;47.15005;,
 -5.22946;-3.06105;-41.40744;,
 -10.45900;2.98086;-35.24775;,
 -5.22946;2.98086;-41.40744;,
 -5.22946;-3.06105;-41.40744;,
 -5.22946;-3.82119;47.15005;,
 -10.45900;-3.82119;33.07900;,
 -10.45900;-3.06105;-35.24775;,
 -5.22946;2.98086;-41.40744;,
 -10.45900;2.98086;-35.24775;,
 -10.45900;3.74099;33.07900;,
 -5.22946;3.74099;47.15005;,
 -5.22946;-3.82119;47.15005;,
 -5.22946;3.74099;47.15005;,
 -10.45900;3.74099;33.07900;,
 0.00008;-2.05703;-47.15159;,
 5.22963;-2.05703;-45.86631;,
 5.22963;1.97685;-45.86631;,
 0.00008;1.97685;-47.15159;,
 0.00008;2.98086;-41.40744;,
 -5.22946;1.97685;-45.86631;,
 -5.22946;-2.05703;-45.86631;,
 28.36009;4.39529;5.46358;,
 28.36009;-4.47549;5.46358;,
 -28.35992;-4.47549;5.46358;,
 -28.35992;4.39529;5.46358;,
 27.52393;-4.47549;20.58974;,
 27.52393;4.39529;20.58974;,
 -27.52376;4.39529;20.58974;,
 -27.52376;-4.47549;20.58974;,
 36.75719;4.39529;13.02663;,
 27.52393;4.39529;20.58974;,
 36.75719;-4.47549;13.02663;,
 28.36009;4.39529;5.46358;,
 36.75719;4.39529;13.02663;,
 28.36009;4.39529;5.46358;,
 -28.35992;4.39529;5.46358;,
 -36.75702;4.39529;13.02663;,
 -27.52376;4.39529;20.58974;,
 27.52393;4.39529;20.58974;,
 27.52393;-4.47549;20.58974;,
 -27.52376;-4.47549;20.58974;,
 -36.75702;-4.47549;13.02663;,
 -28.35992;-4.47549;5.46358;,
 28.36009;-4.47549;5.46358;,
 -36.75702;4.39529;13.02663;,
 -28.35992;4.39529;5.46358;,
 -27.52376;4.39529;20.58974;,
 30.39994;4.53002;15.21649;,
 30.39994;4.53002;10.26736;,
 -29.72726;4.53002;10.26736;,
 -29.72726;4.53002;15.21649;,
 30.39994;4.53002;10.26736;,
 30.39994;-4.61021;10.26736;,
 -29.72726;-4.61021;10.26736;,
 -29.72726;4.53002;10.26736;,
 30.39994;-4.61021;10.26736;,
 30.39994;-4.61021;15.21649;,
 -29.72726;-4.61021;15.21649;,
 -29.72726;-4.61021;10.26736;,
 30.39994;-4.61021;15.21649;,
 30.39994;4.53002;15.21649;,
 -29.72726;4.53002;15.21649;,
 -29.72726;-4.61021;15.21649;,
 30.39994;4.53002;10.26736;,
 30.39994;4.53002;15.21649;,
 -29.72726;4.53002;15.21649;,
 -29.72726;4.53002;10.26736;,
 24.29424;4.53002;8.50321;,
 24.29424;4.53002;6.93948;,
 -24.13764;4.53002;6.93948;,
 -24.13764;4.53002;8.50321;,
 24.29424;4.53002;6.93948;,
 24.29424;-4.61021;6.93948;,
 -24.13764;-4.61021;6.93948;,
 -24.13764;4.53002;6.93948;,
 24.29424;-4.61021;6.93948;,
 24.29424;-4.61021;8.50321;,
 -24.13764;-4.61021;8.50321;,
 -24.13764;-4.61021;6.93948;,
 24.29424;-4.61021;8.50321;,
 24.29424;4.53002;8.50321;,
 -24.13764;4.53002;8.50321;,
 -24.13764;-4.61021;8.50321;,
 24.29424;4.53002;6.93948;,
 24.29424;4.53002;8.50321;,
 -24.13764;4.53002;8.50321;,
 -24.13764;4.53002;6.93948;,
 24.41528;4.53002;18.62358;,
 24.41528;4.53002;17.05979;,
 -24.25865;4.53002;17.05979;,
 -24.25865;4.53002;18.62358;,
 24.41528;4.53002;17.05979;,
 24.41528;-4.61021;17.05979;,
 -24.25865;-4.61021;17.05979;,
 -24.25865;4.53002;17.05979;,
 24.41528;-4.61021;17.05979;,
 24.41528;-4.61021;18.62358;,
 -24.25865;-4.61021;18.62358;,
 -24.25865;-4.61021;17.05979;,
 24.41528;-4.61021;18.62358;,
 24.41528;4.53002;18.62358;,
 -24.25865;4.53002;18.62358;,
 -24.25865;-4.61021;18.62358;,
 24.41528;4.53002;17.05979;,
 24.41528;4.53002;18.62358;,
 -24.25865;4.53002;18.62358;,
 -24.25865;4.53002;17.05979;,
 0.00008;4.86861;9.84974;,
 1.85781;4.86861;10.61927;,
 1.85781;-4.94449;10.61927;,
 0.00008;-4.94449;9.84974;,
 2.62729;4.86861;12.47700;,
 2.62729;-4.94449;12.47700;,
 1.85781;4.86861;14.33470;,
 1.85781;-4.94449;14.33470;,
 0.00008;4.86861;15.10424;,
 0.00008;-4.94449;15.10424;,
 -1.85764;4.86861;14.33470;,
 -1.85764;-4.94449;14.33470;,
 -2.62712;4.86861;12.47700;,
 -2.62712;-4.94449;12.47700;,
 -1.85764;4.86861;10.61927;,
 -1.85764;-4.94449;10.61927;,
 0.00008;4.86861;9.84974;,
 0.00008;-4.94449;9.84974;,
 0.00008;4.86861;12.47700;,
 0.00008;4.86861;12.47700;,
 0.00008;4.86861;12.47700;,
 0.00008;4.86861;12.47700;,
 0.00008;4.86861;12.47700;,
 0.00008;4.86861;12.47700;,
 0.00008;4.86861;12.47700;,
 0.00008;4.86861;12.47700;,
 0.00008;-4.94449;12.47700;,
 0.00008;-4.94449;12.47700;,
 0.00008;-4.94449;12.47700;,
 0.00008;-4.94449;12.47700;,
 0.00008;-4.94449;12.47700;,
 0.00008;-4.94449;12.47700;,
 0.00008;-4.94449;12.47700;,
 0.00008;-4.94449;12.47700;,
 29.81042;3.91458;4.37329;,
 29.81042;-3.99477;4.37329;,
 -29.81025;-3.99477;4.37329;,
 -29.81025;3.91458;4.37329;,
 28.93155;-3.99478;21.68001;,
 28.93155;3.91458;21.68001;,
 -28.93133;3.91458;21.68001;,
 -28.93133;-3.99478;21.68001;,
 38.63694;3.91458;13.02663;,
 28.93155;3.91458;21.68001;,
 38.63694;-3.99478;13.02663;,
 29.81042;3.91458;4.37329;,
 38.63694;3.91458;13.02663;,
 29.81042;3.91458;4.37329;,
 -29.81025;3.91458;4.37329;,
 -38.63677;3.91458;13.02663;,
 -28.93133;3.91458;21.68001;,
 28.93155;3.91458;21.68001;,
 28.93155;-3.99478;21.68001;,
 -28.93133;-3.99478;21.68001;,
 -38.63677;-3.99478;13.02663;,
 -29.81025;-3.99477;4.37329;,
 29.81042;-3.99477;4.37329;,
 -38.63677;3.91458;13.02663;,
 -29.81025;3.91458;4.37329;,
 -28.93133;3.91458;21.68001;,
 8.96089;4.39529;32.26431;,
 8.96089;-4.47549;32.26431;,
 8.96089;-3.58380;-34.37965;,
 8.96089;3.50361;-34.37965;,
 -8.96072;-4.47549;32.26431;,
 -8.96072;4.39529;32.26431;,
 -8.96072;3.50361;-34.37965;,
 -8.96072;-3.58380;-34.37965;,
 4.48047;-3.58380;-40.38767;,
 4.48047;3.50361;-40.38767;,
 8.96089;3.50361;-34.37965;,
 4.48047;-3.58380;-40.38767;,
 8.96089;-3.58380;-34.37965;,
 8.96089;-4.47549;32.26431;,
 4.48047;-4.47549;45.98878;,
 0.00008;-4.47549;45.98878;,
 0.00008;-3.58380;-40.38767;,
 4.48047;3.50361;-40.38767;,
 0.00008;3.50361;-40.38767;,
 0.00008;4.39529;45.98878;,
 4.48047;4.39529;45.98878;,
 8.96089;4.39529;32.26431;,
 8.96089;3.50361;-34.37965;,
 4.48047;-4.47549;45.98878;,
 8.96089;4.39529;32.26431;,
 4.48047;4.39529;45.98878;,
 0.00008;4.39529;45.98878;,
 -4.48030;-3.58380;-40.38767;,
 -8.96072;3.50361;-34.37965;,
 -4.48030;3.50361;-40.38767;,
 -4.48030;-3.58380;-40.38767;,
 -4.48030;-4.47549;45.98878;,
 -8.96072;-4.47549;32.26431;,
 -8.96072;-3.58380;-34.37965;,
 -4.48030;3.50361;-40.38767;,
 -8.96072;3.50361;-34.37965;,
 -8.96072;4.39529;32.26431;,
 -4.48030;4.39529;45.98878;,
 -4.48030;-4.47549;45.98878;,
 -4.48030;4.39529;45.98878;,
 -8.96072;4.39529;32.26431;,
 0.00008;-2.40605;-45.99028;,
 4.48047;-2.40605;-44.73673;,
 4.48047;2.32587;-44.73673;,
 0.00008;2.32587;-45.99028;,
 0.00008;3.50361;-40.38767;,
 -4.48030;2.32587;-44.73673;,
 -4.48030;-2.40605;-44.73673;,
 -36.75702;4.39529;13.02663;,
 -28.35992;4.39529;5.46358;,
 -28.35992;-4.47549;5.46358;,
 -36.75702;-4.47549;13.02663;,
 -27.52376;-4.47549;20.58974;,
 -27.52376;4.39529;20.58974;,
 -2.25076;4.88505;43.68318;,
 2.25093;4.88505;43.68318;,
 2.25093;3.89489;-38.08201;,
 -2.25076;3.89489;-38.08201;,
 2.25093;4.88505;43.68318;,
 2.25093;-4.96521;43.68318;,
 2.25093;-3.97508;-38.08201;,
 2.25093;3.89489;-38.08201;,
 2.25093;-4.96521;43.68318;,
 -2.25076;-4.96521;43.68318;,
 -2.25076;-3.97508;-38.08201;,
 2.25093;-3.97508;-38.08201;,
 -2.25076;-4.96521;43.68318;,
 -2.25076;4.88505;43.68318;,
 -2.25076;3.89489;-38.08201;,
 -2.25076;-3.97508;-38.08201;,
 2.25093;4.88505;43.68318;,
 -2.25076;4.88505;43.68318;,
 -2.25076;3.89489;-38.08201;,
 2.25093;3.89489;-38.08201;,
 3.91597;4.93441;38.17769;,
 5.47972;4.93441;38.17769;,
 5.47972;3.93435;-32.57650;,
 3.91597;3.93435;-32.57650;,
 5.47972;4.93441;38.17769;,
 5.47972;-5.01461;38.17769;,
 5.47972;-4.01454;-32.57650;,
 5.47972;3.93435;-32.57650;,
 5.47972;-5.01461;38.17769;,
 3.91597;-5.01461;38.17769;,
 3.91597;-4.01454;-32.57650;,
 5.47972;-4.01454;-32.57650;,
 3.91597;-5.01461;38.17769;,
 3.91597;4.93441;38.17769;,
 3.91597;3.93435;-32.57650;,
 3.91597;-4.01454;-32.57650;,
 5.47972;4.93441;38.17769;,
 3.91597;4.93441;38.17769;,
 3.91597;3.93435;-32.57650;,
 5.47972;3.93435;-32.57650;,
 -5.75905;5.06004;38.17769;,
 -4.19526;5.06004;38.17769;,
 -4.19526;4.03472;-32.57650;,
 -5.75905;4.03472;-32.57650;,
 -4.19526;5.06004;38.17769;,
 -4.19526;-5.14021;38.17769;,
 -4.19526;-4.11490;-32.57650;,
 -4.19526;4.03472;-32.57650;,
 -4.19526;-5.14021;38.17769;,
 -5.75905;-5.14021;38.17769;,
 -5.75905;-4.11490;-32.57650;,
 -4.19526;-4.11490;-32.57650;,
 -5.75905;-5.14021;38.17769;,
 -5.75905;5.06004;38.17769;,
 -5.75905;4.03472;-32.57650;,
 -5.75905;-4.11490;-32.57650;,
 -4.19526;5.06004;38.17769;,
 -5.75905;5.06004;38.17769;,
 -5.75905;4.03472;-32.57650;,
 -4.19526;4.03472;-32.57650;,
 -29.72726;4.53002;15.21649;,
 -29.72726;4.53002;10.26736;,
 -29.72726;-4.61021;10.26736;,
 -29.72726;-4.61021;15.21649;,
 24.29424;4.53002;8.50321;,
 24.29424;4.53002;6.93948;,
 -24.13764;4.53002;6.93948;,
 -24.13764;4.53002;8.50321;,
 24.29424;4.53002;6.93948;,
 24.29424;-4.61021;6.93948;,
 -24.13764;-4.61021;6.93948;,
 -24.13764;4.53002;6.93948;,
 24.29424;-4.61021;6.93948;,
 24.29424;-4.61021;8.50321;,
 -24.13764;-4.61021;8.50321;,
 -24.13764;-4.61021;6.93948;,
 24.29424;-4.61021;8.50321;,
 24.29424;4.53002;8.50321;,
 -24.13764;4.53002;8.50321;,
 -24.13764;-4.61021;8.50321;,
 24.29424;4.53002;6.93948;,
 24.29424;4.53002;8.50321;,
 -24.13764;4.53002;8.50321;,
 -24.13764;4.53002;6.93948;,
 0.00008;4.86861;9.84974;,
 1.85781;4.86861;10.61927;,
 1.85781;-4.94449;10.61927;,
 0.00008;-4.94449;9.84974;,
 2.62729;4.86861;12.47700;,
 2.62729;-4.94449;12.47700;,
 1.85781;4.86861;14.33470;,
 1.85781;-4.94449;14.33470;,
 0.00008;4.86861;15.10424;,
 0.00008;-4.94449;15.10424;,
 -1.85764;4.86861;14.33470;,
 -1.85764;-4.94449;14.33470;,
 -2.62712;4.86861;12.47700;,
 -2.62712;-4.94449;12.47700;,
 -1.85764;4.86861;10.61927;,
 -1.85764;-4.94449;10.61927;,
 0.00008;4.86861;9.84974;,
 0.00008;-4.94449;9.84974;,
 0.00008;4.86861;12.47700;,
 0.00008;4.86861;12.47700;,
 0.00008;4.86861;12.47700;,
 0.00008;4.86861;12.47700;,
 0.00008;4.86861;12.47700;,
 0.00008;4.86861;12.47700;,
 0.00008;4.86861;12.47700;,
 0.00008;4.86861;12.47700;,
 0.00008;-4.94449;12.47700;,
 0.00008;-4.94449;12.47700;,
 0.00008;-4.94449;12.47700;,
 0.00008;-4.94449;12.47700;,
 0.00008;-4.94449;12.47700;,
 0.00008;-4.94449;12.47700;,
 0.00008;-4.94449;12.47700;,
 0.00008;-4.94449;12.47700;;
 
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
 4;176,177,172,178;,
 4;176,178,169,179;,
 4;180,181,182,183;,
 4;180,183,184,185;,
 4;178,186,187,188;,
 4;178,188,189,190;,
 4;191,192,170,188;,
 4;191,188,175,193;,
 4;194,195,196,197;,
 4;198,199,200,201;,
 4;202,203,204,196;,
 4;205,206,207,208;,
 4;205,208,209,210;,
 4;211,212,213,214;,
 4;211,214,215,216;,
 4;217,195,218,219;,
 4;217,219,220,209;,
 4;221,201,222,223;,
 4;224,210,209,225;,
 4;224,225,226,227;,
 4;228,229,230,231;,
 4;228,231,213,212;,
 4;232,209,220,233;,
 4;232,233,234,198;,
 4;235,236,202,210;,
 4;237,238,239,203;,
 4;236,237,203,202;,
 4;236,235,238,237;,
 4;240,241,221,223;,
 4;238,240,223,239;,
 4;241,235,210,221;,
 4;241,240,238,235;,
 4;242,243,244,245;,
 4;242,245,246,247;,
 4;248,249,250,251;,
 4;252,253,254,255;,
 4;256,257,258,259;,
 4;260,261,262,263;,
 4;260,253,264,265;,
 4;266,267,254,263;,
 4;268,269,270,271;,
 4;272,273,274,275;,
 4;276,277,278,279;,
 4;280,281,282,283;,
 4;280,273,284,285;,
 4;286,287,274,283;,
 4;288,289,290,291;,
 4;292,293,294,295;,
 4;296,297,298,299;,
 4;300,301,302,303;,
 4;300,293,304,305;,
 4;306,307,294,303;,
 4;308,309,310,311;,
 4;312,313,314,315;,
 4;316,317,318,319;,
 4;320,321,322,323;,
 4;324,325,326,327;,
 4;324,317,328,329;,
 4;330,331,318,327;,
 4;332,333,334,335;,
 4;333,336,337,334;,
 4;336,338,339,337;,
 4;338,340,341,339;,
 4;340,342,343,341;,
 4;342,344,345,343;,
 4;344,346,347,345;,
 4;346,348,349,347;,
 3;350,333,332;,
 3;351,336,333;,
 3;352,338,336;,
 3;353,340,338;,
 3;354,342,340;,
 3;355,344,342;,
 3;356,346,344;,
 3;357,348,346;,
 3;358,335,334;,
 3;359,334,337;,
 3;360,337,339;,
 3;361,339,341;,
 3;362,341,343;,
 3;363,343,345;,
 3;364,345,347;,
 3;365,347,349;;
 
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
  2;;
  Material {
   0.000000;0.016000;0.800000;1.000000;;
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
   0.746400;0.800000;0.000000;1.000000;;
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
  100;
  -0.984215;0.000000;0.176979;,
  -0.938701;0.000000;-0.344734;,
  0.984215;0.000000;0.176979;,
  0.938701;0.000000;-0.344734;,
  -0.000000;-0.994709;-0.102735;,
  -0.000000;0.994709;-0.102734;,
  0.000000;-0.999963;-0.008583;,
  0.000000;0.999963;-0.008583;,
  -0.002497;-0.999954;-0.009219;,
  -0.002497;0.999954;-0.009218;,
  0.002497;-0.999954;-0.009219;,
  0.002497;0.999954;-0.009218;,
  -0.000000;-0.980605;-0.195995;,
  -0.000000;0.980605;-0.195993;,
  0.021151;-0.980386;-0.195951;,
  0.021150;0.980386;-0.195949;,
  -1.000000;0.000000;0.000000;,
  0.336450;0.000000;0.941701;,
  -0.336450;0.000000;0.941701;,
  0.358438;0.000000;-0.933554;,
  -0.358438;0.000000;-0.933554;,
  0.633671;0.000000;0.773603;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  -0.000001;0.000000;-1.000000;,
  0.707115;0.000000;-0.707099;,
  1.000000;0.000000;0.000003;,
  0.707115;0.000000;0.707099;,
  -0.000001;0.000000;1.000000;,
  -0.707115;0.000000;0.707098;,
  -1.000000;0.000000;0.000003;,
  -0.707116;0.000000;-0.707098;,
  0.356089;0.000000;0.934452;,
  -0.356088;0.000000;0.934453;,
  0.378102;0.000000;-0.925764;,
  -0.378102;0.000000;-0.925764;,
  0.665496;0.000000;0.746402;,
  0.000000;-1.000000;-0.000001;,
  -0.987580;0.000000;0.157120;,
  -0.949114;0.000000;-0.314932;,
  0.987580;0.000000;0.157120;,
  0.949114;0.000000;-0.314932;,
  -0.000000;-0.992437;-0.122755;,
  -0.000000;0.992437;-0.122754;,
  0.000000;-0.999947;-0.010323;,
  0.000000;0.999947;-0.010323;,
  -0.003418;-0.999933;-0.011087;,
  -0.003418;0.999933;-0.011087;,
  0.003418;-0.999933;-0.011087;,
  0.003418;0.999933;-0.011087;,
  -0.000000;-0.972315;-0.233673;,
  -0.000000;0.972316;-0.233671;,
  0.028755;-0.971913;-0.233577;,
  0.028754;0.971914;-0.233575;,
  -0.633671;0.000000;0.773603;,
  -0.669241;0.000000;-0.743045;,
  0.000000;0.999927;-0.012109;,
  1.000000;0.000000;0.000000;,
  0.000000;-0.999927;-0.012109;,
  0.000000;0.999900;-0.014133;,
  0.000000;-0.999900;-0.014133;,
  0.000000;0.999895;-0.014490;,
  0.000000;-0.999895;-0.014490;,
  0.005407;-0.997408;-0.071749;,
  -0.004994;-0.999939;-0.009854;,
  0.005407;0.997408;-0.071748;,
  -0.004994;0.999939;-0.009854;,
  0.937357;0.000000;0.348371;,
  0.000000;0.000000;1.000000;,
  -0.005407;-0.997408;-0.071749;,
  0.004994;-0.999939;-0.009854;,
  -0.005407;0.997408;-0.071748;,
  0.004994;0.999939;-0.009854;,
  -0.937357;0.000000;0.348371;,
  0.238670;0.000000;-0.971101;,
  -0.000000;0.000000;-1.000000;,
  -0.021150;0.980386;-0.195949;,
  -0.021151;-0.980386;-0.195951;,
  -0.238671;0.000000;-0.971101;,
  0.669241;0.000000;-0.743045;,
  0.700067;0.000000;-0.714077;,
  0.000000;-1.000000;-0.000001;,
  -0.700067;0.000000;-0.714077;,
  -0.665494;0.000000;0.746403;,
  0.007348;-0.996290;-0.085741;,
  -0.006836;-0.999906;-0.011851;,
  0.007348;0.996290;-0.085740;,
  -0.006836;0.999906;-0.011851;,
  0.950627;0.000000;0.310337;,
  -0.007348;-0.996290;-0.085741;,
  0.006836;-0.999906;-0.011851;,
  -0.007348;0.996290;-0.085740;,
  0.006836;0.999906;-0.011851;,
  -0.950627;0.000000;0.310337;,
  0.269439;0.000000;-0.963018;,
  -0.000000;0.000000;-1.000000;,
  -0.028754;0.971914;-0.233575;,
  -0.028755;-0.971913;-0.233577;,
  -0.269439;0.000000;-0.963017;;
  161;
  4;2,2,3,3;,
  4;0,0,1,1;,
  4;3,3,3,3;,
  4;64,65,65,8;,
  4;64,8,6,4;,
  4;66,5,7,9;,
  4;66,9,67,67;,
  4;68,2,2,68;,
  4;69,69,69,69;,
  4;1,1,1,1;,
  4;70,4,6,10;,
  4;70,10,71,71;,
  4;72,73,73,11;,
  4;72,11,7,5;,
  4;69,69,69,69;,
  4;74,74,0,0;,
  4;12,14,64,4;,
  4;15,13,5,66;,
  4;58,58,3,3;,
  4;75,76,76,75;,
  4;16,16,1,1;,
  4;13,77,72,5;,
  4;78,12,4,70;,
  4;79,79,76,76;,
  4;19,19,20,20;,
  4;17,17,18,18;,
  4;21,17,17,21;,
  4;80,80,19,19;,
  4;22,22,22,22;,
  4;22,22,22,22;,
  4;23,23,23,23;,
  4;23,23,23,23;,
  4;56,20,20,56;,
  4;55,55,18,18;,
  4;22,22,22,22;,
  4;24,24,24,24;,
  4;23,23,23,23;,
  4;69,69,69,69;,
  4;58,58,58,58;,
  4;16,16,16,16;,
  4;22,22,22,22;,
  4;24,24,24,24;,
  4;23,23,23,23;,
  4;69,69,69,69;,
  4;58,58,58,58;,
  4;16,16,16,16;,
  4;22,22,22,22;,
  4;24,24,24,24;,
  4;23,23,23,23;,
  4;69,69,69,69;,
  4;58,58,58,58;,
  4;16,16,16,16;,
  4;25,26,26,25;,
  4;26,27,27,26;,
  4;27,28,28,27;,
  4;28,29,29,28;,
  4;29,30,30,29;,
  4;30,31,31,30;,
  4;31,32,32,31;,
  4;32,25,25,32;,
  3;22,22,22;,
  3;22,22,22;,
  3;22,22,22;,
  3;22,22,22;,
  3;22,22,22;,
  3;22,22,22;,
  3;22,22,22;,
  3;22,22,22;,
  3;23,23,23;,
  3;23,23,23;,
  3;23,23,23;,
  3;23,23,23;,
  3;23,23,23;,
  3;23,23,23;,
  3;23,23,23;,
  3;23,23,23;,
  4;35,35,36,36;,
  4;33,33,34,34;,
  4;37,33,33,37;,
  4;81,81,35,35;,
  4;22,22,22,22;,
  4;22,22,22,22;,
  4;38,23,23,38;,
  4;38,38,82,82;,
  4;83,36,36,83;,
  4;84,84,34,34;,
  4;41,41,42,42;,
  4;39,39,40,40;,
  4;42,42,42,42;,
  4;85,86,86,47;,
  4;85,47,45,43;,
  4;87,44,46,48;,
  4;87,48,88,88;,
  4;89,41,41,89;,
  4;69,69,69,69;,
  4;40,40,40,40;,
  4;90,43,45,49;,
  4;90,49,91,91;,
  4;92,93,93,50;,
  4;92,50,46,44;,
  4;69,69,69,69;,
  4;94,94,39,39;,
  4;51,53,85,43;,
  4;54,52,44,87;,
  4;58,58,42,42;,
  4;95,96,96,95;,
  4;16,16,40,40;,
  4;52,97,92,44;,
  4;98,51,43,90;,
  4;99,99,96,96;,
  4;56,56,56,56;,
  4;55,55,55,55;,
  4;57,57,57,57;,
  4;58,58,58,58;,
  4;59,59,59,59;,
  4;16,16,16,16;,
  4;69,69,69,69;,
  4;24,24,24,24;,
  4;60,60,60,60;,
  4;58,58,58,58;,
  4;61,61,61,61;,
  4;16,16,16,16;,
  4;69,69,69,69;,
  4;24,24,24,24;,
  4;62,62,62,62;,
  4;58,58,58,58;,
  4;63,63,63,63;,
  4;16,16,16,16;,
  4;69,69,69,69;,
  4;24,24,24,24;,
  4;16,16,16,16;,
  4;22,22,22,22;,
  4;24,24,24,24;,
  4;23,23,23,23;,
  4;69,69,69,69;,
  4;58,58,58,58;,
  4;16,16,16,16;,
  4;25,26,26,25;,
  4;26,27,27,26;,
  4;27,28,28,27;,
  4;28,29,29,28;,
  4;29,30,30,29;,
  4;30,31,31,30;,
  4;31,32,32,31;,
  4;32,25,25,32;,
  3;22,22,22;,
  3;22,22,22;,
  3;22,22,22;,
  3;22,22,22;,
  3;22,22,22;,
  3;22,22,22;,
  3;22,22,22;,
  3;22,22,22;,
  3;23,23,23;,
  3;23,23,23;,
  3;23,23,23;,
  3;23,23,23;,
  3;23,23,23;,
  3;23,23,23;,
  3;23,23,23;,
  3;23,23,23;;
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
  0.937500;1.000000;;
 }
}
