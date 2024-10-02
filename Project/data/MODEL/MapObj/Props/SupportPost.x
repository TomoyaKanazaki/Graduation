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
 28;
 2.89569;-0.55036;5.44086;,
 -1.11811;-0.55036;5.44086;,
 -1.11811;-123.20391;5.44086;,
 2.89569;-123.20391;5.44086;,
 -1.11811;-0.55036;-5.17460;,
 2.89569;-0.55036;-5.17460;,
 2.89569;-123.20391;-5.17460;,
 -1.11811;-123.20391;-5.17460;,
 5.01271;-0.55036;0.71990;,
 2.89569;-0.55036;5.44086;,
 2.89569;-123.20391;5.44086;,
 5.01271;-123.20391;0.77540;,
 2.89569;-123.20391;-5.17460;,
 2.89569;-0.55036;-5.17460;,
 -4.76071;-0.55036;0.70545;,
 -1.11811;-0.55036;-5.17460;,
 -1.11811;-123.20391;-5.17460;,
 -4.76071;-123.20391;0.76096;,
 -1.11811;-123.20391;5.44086;,
 -1.11811;-0.55036;5.44086;,
 -4.76071;-0.55036;0.70545;,
 2.89569;-0.55036;5.44086;,
 5.01271;-0.55036;0.71990;,
 2.89569;-0.55036;-5.17460;,
 5.01271;-123.20391;0.77540;,
 2.89569;-123.20391;5.44086;,
 -4.76071;-123.20391;0.76096;,
 2.89569;-123.20391;-5.17460;;
 
 10;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;8,11,12,13;,
 4;14,15,16,17;,
 4;14,17,18,19;,
 4;20,1,21,22;,
 4;20,22,23,4;,
 4;24,25,2,26;,
 4;24,26,7,27;;
 
 MeshMaterialList {
  1;
  10;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MapObj\\wood_1004.jpg";
   }
  }
 }
 MeshNormals {
  11;
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;1.000000;,
  -0.440473;0.000077;0.897766;,
  -0.487423;-0.000068;-0.873166;,
  0.999191;0.000009;0.040211;,
  -0.851202;-0.000119;-0.524838;,
  0.911548;0.000093;0.411194;,
  0.941643;-0.000076;-0.336614;,
  -0.790883;0.000139;0.611968;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;;
  10;
  4;1,2,2,1;,
  4;3,0,0,3;,
  4;4,6,6,4;,
  4;4,4,7,7;,
  4;5,3,3,5;,
  4;8,8,2,2;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;10,10,10,10;,
  4;10,10,10,10;;
 }
 MeshTextureCoords {
  28;
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.555280;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.560500;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.446090;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.440860;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.553910;0.000000;,
  1.000000;1.000000;,
  0.555280;1.000000;,
  0.000000;1.000000;,
  0.560500;0.000000;,
  1.000000;0.000000;,
  0.559140;1.000000;,
  0.000000;0.000000;;
 }
}
