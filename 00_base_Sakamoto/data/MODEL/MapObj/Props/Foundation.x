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
 96;
 -934.10541;-1.82462;-1296.95251;,
 934.10541;-1.82462;-1296.95251;,
 934.10541;-766.89398;-1296.95251;,
 -934.10541;-766.89398;-1296.95251;,
 934.10541;-1.82462;-1296.95251;,
 934.10541;-1.82462;-1096.95251;,
 934.10541;-766.89398;-1096.95251;,
 934.10541;-766.89398;-1296.95251;,
 934.10541;-1.82462;-1096.95251;,
 -934.10541;-1.82462;-1096.95251;,
 -934.10541;-766.89398;-1096.95251;,
 934.10541;-766.89398;-1096.95251;,
 -934.10541;-1.82462;-1096.95251;,
 -934.10541;-1.82462;-1296.95251;,
 -934.10541;-766.89398;-1296.95251;,
 -934.10541;-766.89398;-1096.95251;,
 -934.10541;-1.82462;-1096.95251;,
 934.10541;-1.82462;-1096.95251;,
 934.10541;-1.82462;-1296.95251;,
 -934.10541;-1.82462;-1296.95251;,
 -934.10541;-766.89398;-1296.95251;,
 934.10541;-766.89398;-1296.95251;,
 934.10541;-766.89398;-1096.95251;,
 -934.10541;-766.89398;-1096.95251;,
 -934.10541;-1.82462;1060.70483;,
 934.10541;-1.82462;1060.70483;,
 934.10541;-766.89398;1060.70483;,
 -934.10541;-766.89398;1060.70483;,
 934.10541;-1.82462;1060.70483;,
 934.10541;-1.82462;1260.70483;,
 934.10541;-766.89398;1260.70483;,
 934.10541;-766.89398;1060.70483;,
 934.10541;-1.82462;1260.70483;,
 -934.10541;-1.82462;1260.70483;,
 -934.10541;-766.89398;1260.70483;,
 934.10541;-766.89398;1260.70483;,
 -934.10541;-1.82462;1260.70483;,
 -934.10541;-1.82462;1060.70483;,
 -934.10541;-766.89398;1060.70483;,
 -934.10541;-766.89398;1260.70483;,
 -934.10541;-1.82462;1260.70483;,
 934.10541;-1.82462;1260.70483;,
 934.10541;-1.82462;1060.70483;,
 -934.10541;-1.82462;1060.70483;,
 -934.10541;-766.89398;1060.70483;,
 934.10541;-766.89398;1060.70483;,
 934.10541;-766.89398;1260.70483;,
 -934.10541;-766.89398;1260.70483;,
 866.78333;-1.82462;1249.59155;,
 866.78326;-1.82462;-1295.60327;,
 866.78326;-766.89398;-1295.60327;,
 866.78333;-766.89398;1249.59155;,
 866.78326;-1.82462;-1295.60327;,
 1066.78320;-1.82462;-1289.27197;,
 1066.78320;-766.89398;-1289.27197;,
 866.78326;-766.89398;-1295.60327;,
 1066.78320;-1.82462;-1289.27197;,
 1066.78333;-1.82462;1243.26025;,
 1066.78333;-766.89398;1243.26025;,
 1066.78320;-766.89398;-1289.27197;,
 1066.78333;-1.82462;1243.26025;,
 866.78333;-1.82462;1249.59155;,
 866.78333;-766.89398;1249.59155;,
 1066.78333;-766.89398;1243.26025;,
 1066.78333;-1.82462;1243.26025;,
 1066.78320;-1.82462;-1289.27197;,
 866.78326;-1.82462;-1295.60327;,
 866.78333;-1.82462;1249.59155;,
 866.78333;-766.89398;1249.59155;,
 866.78326;-766.89398;-1295.60327;,
 1066.78320;-766.89398;-1289.27197;,
 1066.78333;-766.89398;1243.26025;,
 -1071.59326;-1.82462;1255.95630;,
 -1071.59326;-1.82462;-1301.96802;,
 -1071.59326;-766.89398;-1301.96802;,
 -1071.59326;-766.89398;1255.95630;,
 -1071.59326;-1.82462;-1301.96802;,
 -871.59320;-1.82462;-1301.96802;,
 -871.59320;-766.89398;-1301.96802;,
 -1071.59326;-766.89398;-1301.96802;,
 -871.59320;-1.82462;-1301.96802;,
 -871.59308;-1.82462;1255.95630;,
 -871.59308;-766.89398;1255.95630;,
 -871.59320;-766.89398;-1301.96802;,
 -871.59308;-1.82462;1255.95630;,
 -1071.59326;-1.82462;1255.95630;,
 -1071.59326;-766.89398;1255.95630;,
 -871.59308;-766.89398;1255.95630;,
 -871.59308;-1.82462;1255.95630;,
 -871.59320;-1.82462;-1301.96802;,
 -1071.59326;-1.82462;-1301.96802;,
 -1071.59326;-1.82462;1255.95630;,
 -1071.59326;-766.89398;1255.95630;,
 -1071.59326;-766.89398;-1301.96802;,
 -871.59320;-766.89398;-1301.96802;,
 -871.59308;-766.89398;1255.95630;;
 
 24;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,21,22,23;,
 4;24,25,26,27;,
 4;28,29,30,31;,
 4;32,33,34,35;,
 4;36,37,38,39;,
 4;40,41,42,43;,
 4;44,45,46,47;,
 4;48,49,50,51;,
 4;52,53,54,55;,
 4;56,57,58,59;,
 4;60,61,62,63;,
 4;64,65,66,67;,
 4;68,69,70,71;,
 4;72,73,74,75;,
 4;76,77,78,79;,
 4;80,81,82,83;,
 4;84,85,86,87;,
 4;88,89,90,91;,
 4;92,93,94,95;;
 
 MeshMaterialList {
  2;
  24;
  0,
  0,
  0,
  0,
  1,
  1,
  0,
  0,
  0,
  0,
  1,
  1,
  0,
  0,
  0,
  0,
  1,
  1,
  0,
  0,
  0,
  0,
  1,
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MapObj\\dodai001.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MapObj\\dodai001.jpg";
   }
  }
 }
 MeshNormals {
  24;
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  -1.000000;0.000000;0.000000;,
  0.031641;0.000000;-0.999499;,
  1.000000;0.000000;-0.000000;,
  0.031641;0.000000;0.999499;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;-0.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;;
  24;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;6,6,6,6;,
  4;7,7,7,7;,
  4;8,8,8,8;,
  4;9,9,9,9;,
  4;10,10,10,10;,
  4;11,11,11,11;,
  4;12,12,12,12;,
  4;13,13,13,13;,
  4;14,14,14,14;,
  4;15,15,15,15;,
  4;16,16,16,16;,
  4;17,17,17,17;,
  4;18,18,18,18;,
  4;19,19,19,19;,
  4;20,20,20,20;,
  4;21,21,21,21;,
  4;22,22,22,22;,
  4;23,23,23,23;;
 }
 MeshTextureCoords {
  96;
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
  -0.335746;2.734828;,
  1.335746;2.734828;,
  1.335746;3.142295;,
  -0.335746;3.142295;,
  -0.335746;3.128690;,
  1.335746;3.128690;,
  1.335746;2.721223;,
  -0.335746;2.721223;,
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
  -0.335746;-2.068511;,
  1.335746;-2.068511;,
  1.335746;-1.661044;,
  -0.335746;-1.661044;,
  -0.335746;-1.674649;,
  1.335746;-1.674649;,
  1.335746;-2.082116;,
  -0.335746;-2.082116;,
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
  1.454453;-2.032971;,
  1.454453;3.126647;,
  1.275513;3.139546;,
  1.275513;-2.045870;,
  1.275513;-2.059474;,
  1.275513;3.125942;,
  1.454453;3.113043;,
  1.454453;-2.046575;,
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
  -0.279816;-2.058837;,
  -0.279816;3.152513;,
  -0.458757;3.152513;,
  -0.458757;-2.058837;,
  -0.458757;-2.072441;,
  -0.458757;3.138909;,
  -0.279816;3.138909;,
  -0.279816;-2.072441;;
 }
}