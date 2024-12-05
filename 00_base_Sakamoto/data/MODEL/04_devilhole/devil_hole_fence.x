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
 58.82875;51.57776;29.62700;,
 -58.89371;51.57776;29.62699;,
 -48.97485;0.83043;16.13128;,
 48.90991;0.83043;16.13129;,
 -48.97485;33.16733;-16.19539;,
 48.90993;33.16733;-16.19539;,
 48.90993;0.83043;-16.19539;,
 -48.97485;0.83043;-16.19539;,
 -58.89370;51.57776;13.46364;,
 -58.89371;51.57776;29.62699;,
 58.82875;51.57776;29.62700;,
 58.82872;51.57776;13.46364;,
 48.90993;33.16733;-16.19539;,
 -48.97485;33.16733;-16.19539;,
 -58.89370;51.57776;13.46364;,
 -48.97485;0.83043;-16.19539;,
 -48.97485;0.83043;-0.03205;,
 -48.97485;0.83043;16.13128;,
 -58.89371;51.57776;29.62699;,
 58.82872;51.57776;13.46364;,
 58.82875;51.57776;29.62700;,
 48.90991;0.83043;16.13129;,
 48.90991;0.83043;-0.03203;,
 48.90993;0.83043;-16.19539;,
 -48.97485;0.83043;-0.03205;,
 48.90991;0.83043;-0.03203;,
 48.90991;0.83043;16.13129;,
 -48.97485;0.83043;16.13128;;
 
 10;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;8,11,12,13;,
 4;14,13,15,16;,
 4;14,16,17,18;,
 4;19,20,21,22;,
 4;19,22,23,12;,
 4;24,15,23,25;,
 4;24,25,26,27;;
 
 MeshMaterialList {
  4;
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
   0.800000;0.097600;0.000000;1.000000;;
   5.000000;
   0.300000;0.300000;0.300000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
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
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  12;
  -0.000000;-0.257006;0.966410;,
  0.000000;0.486111;-0.873897;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.961671;-0.274206;,
  -0.989899;-0.127397;-0.062209;,
  0.989899;-0.127397;-0.062209;,
  0.000000;1.000000;0.000000;,
  -0.990358;-0.061938;-0.123915;,
  -0.981429;-0.191826;-0.000000;,
  0.981429;-0.191825;-0.000001;,
  0.990358;-0.061938;-0.123913;,
  0.000000;-1.000000;-0.000000;;
  10;
  4;0,0,0,0;,
  4;1,1,2,2;,
  4;3,6,6,3;,
  4;3,3,1,1;,
  4;4,7,7,4;,
  4;4,4,8,8;,
  4;5,9,9,5;,
  4;5,5,10,10;,
  4;11,11,11,11;,
  4;11,11,11,11;;
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
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.500000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.500000;,
  0.000000;0.500000;,
  0.000000;0.000000;,
  1.000000;0.000000;;
 }
}
