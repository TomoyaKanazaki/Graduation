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
 24;
 9.98984;284.74200;-54.85116;,
 -8.01016;284.74200;-54.85116;,
 -8.01022;0.00000;4.95484;,
 9.98984;0.00000;4.95484;,
 -8.01016;284.74200;-54.85116;,
 -8.01016;284.74200;-65.20396;,
 -8.01022;0.00000;-5.39792;,
 -8.01022;0.00000;4.95484;,
 -8.01016;284.74200;-65.20396;,
 9.98984;284.74200;-65.20396;,
 9.98984;0.00000;-5.39792;,
 -8.01022;0.00000;-5.39792;,
 9.98984;284.74200;-65.20396;,
 9.98984;284.74200;-54.85116;,
 9.98984;0.00000;4.95484;,
 9.98984;0.00000;-5.39792;,
 9.98984;284.74200;-65.20396;,
 -8.01016;284.74200;-65.20396;,
 -8.01016;284.74200;-54.85116;,
 9.98984;284.74200;-54.85116;,
 9.98984;0.00000;4.95484;,
 -8.01022;0.00000;4.95484;,
 -8.01022;0.00000;-5.39792;,
 9.98984;0.00000;-5.39792;;
 
 6;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,21,22,23;;
 
 MeshMaterialList {
  9;
  6;
  6,
  6,
  6,
  6,
  6,
  6;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\woodboard001.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\woodboard001.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\woodboard001.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\woodboard002.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\woodboard002.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\woodboard002.jpg";
   }
  }
  Material {
   0.605600;0.605600;0.605600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\wood001.jpg";
   }
  }
  Material {
   0.800000;0.031200;0.000000;1.000000;;
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
  6;
  0.000000;0.205551;0.978647;,
  -1.000000;0.000000;0.000000;,
  0.000000;-0.205551;-0.978646;,
  1.000000;0.000000;-0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;0.000000;;
  6;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;5,5,5,5;;
 }
 MeshTextureCoords {
  24;
  0.147450;-0.754810;,
  0.068130;-0.754810;,
  0.068130;0.500000;,
  0.147450;0.500000;,
  0.636800;-1.290500;,
  0.604300;-1.290500;,
  0.792050;0.675230;,
  0.824550;0.675230;,
  0.068130;-0.754810;,
  0.147450;-0.754810;,
  0.147450;0.500000;,
  0.068130;0.500000;,
  0.604300;-1.290500;,
  0.636800;-1.290500;,
  0.824550;0.675230;,
  0.792050;0.675230;,
  0.147440;1.005890;,
  0.068110;1.005890;,
  0.068110;1.041800;,
  0.147440;1.041800;,
  0.121940;2.678550;,
  0.036870;2.678550;,
  0.036870;2.574140;,
  0.121940;2.574140;;
 }
}
