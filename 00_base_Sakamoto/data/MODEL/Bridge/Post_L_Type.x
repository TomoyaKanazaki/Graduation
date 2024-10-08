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
 48;
 9.56150;284.74200;-54.85116;,
 -8.43850;284.74200;-54.85116;,
 -8.43852;0.00000;4.95484;,
 9.56150;0.00000;4.95484;,
 -8.43850;284.74200;-54.85116;,
 -8.43850;284.74200;-65.20396;,
 -8.43852;0.00000;-5.39792;,
 -8.43852;0.00000;4.95484;,
 -8.43850;284.74200;-65.20396;,
 9.56150;284.74200;-65.20396;,
 9.56150;0.00000;-5.39792;,
 -8.43852;0.00000;-5.39792;,
 9.56150;284.74200;-65.20396;,
 9.56150;284.74200;-54.85116;,
 9.56150;0.00000;4.95484;,
 9.56150;0.00000;-5.39792;,
 9.56150;284.74200;-65.20396;,
 -8.43850;284.74200;-65.20396;,
 -8.43850;284.74200;-54.85116;,
 9.56150;284.74200;-54.85116;,
 9.56150;0.00000;4.95484;,
 -8.43852;0.00000;4.95484;,
 -8.43852;0.00000;-5.39792;,
 9.56150;0.00000;-5.39792;,
 -168.43844;280.00000;-62.04516;,
 -8.43850;280.00000;-62.04516;,
 -8.43850;260.00000;-57.04516;,
 -168.43844;260.00000;-57.04516;,
 -8.43850;280.00000;-62.04516;,
 -8.43850;280.00000;-57.04516;,
 -8.43850;260.00000;-53.04516;,
 -8.43850;260.00000;-57.04516;,
 -8.43850;280.00000;-57.04516;,
 -168.43844;280.00000;-57.04516;,
 -168.43844;260.00000;-53.04516;,
 -8.43850;260.00000;-53.04516;,
 -168.43844;280.00000;-57.04516;,
 -168.43844;280.00000;-62.04516;,
 -168.43844;260.00000;-57.04516;,
 -168.43844;260.00000;-53.04516;,
 -168.43844;280.00000;-57.04516;,
 -8.43850;280.00000;-57.04516;,
 -8.43850;280.00000;-62.04516;,
 -168.43844;280.00000;-62.04516;,
 -168.43844;260.00000;-57.04516;,
 -8.43850;260.00000;-57.04516;,
 -8.43850;260.00000;-53.04516;,
 -168.43844;260.00000;-53.04516;;
 
 12;
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
 4;44,45,46,47;;
 
 MeshMaterialList {
  9;
  12;
  6,
  0,
  6,
  6,
  6,
  6,
  6,
  3,
  6,
  3,
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
  9;
  0.000000;0.205551;0.978647;,
  -1.000000;0.000000;0.000000;,
  0.000000;-0.205551;-0.978646;,
  0.000000;-0.242536;-0.970142;,
  1.000000;0.000000;-0.000000;,
  0.000000;0.196116;0.980581;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;0.000000;,
  -1.000000;0.000000;0.000000;;
  12;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;4,4,4,4;,
  4;6,6,6,6;,
  4;7,7,7,7;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;8,8,8,8;,
  4;6,6,6,6;,
  4;7,7,7,7;;
 }
 MeshTextureCoords {
  48;
  0.931870;-0.754810;,
  0.852550;-0.754810;,
  0.852550;0.500000;,
  0.931870;0.500000;,
  -0.025732;-1.290499;,
  -0.058233;-1.290499;,
  0.129517;0.675234;,
  0.162017;0.675234;,
  0.852550;-0.754810;,
  0.931870;-0.754810;,
  0.931870;0.500000;,
  0.852550;0.500000;,
  0.604300;-1.290500;,
  0.636800;-1.290500;,
  0.824550;0.675230;,
  0.792050;0.675230;,
  0.931890;1.005890;,
  0.852560;1.005890;,
  0.852560;1.041800;,
  0.931890;1.041800;,
  0.963130;2.678550;,
  0.878060;2.678550;,
  0.878060;2.574140;,
  0.963130;2.574140;,
  0.147450;-0.733920;,
  0.852550;-0.733920;,
  0.852550;-0.645780;,
  0.147450;-0.645780;,
  0.460121;-0.125781;,
  0.460121;-0.075351;,
  0.460121;-0.035008;,
  0.460121;-0.075351;,
  0.852550;-0.733920;,
  0.147450;-0.733920;,
  0.147450;-0.645780;,
  0.852550;-0.645780;,
  -0.296003;-0.075351;,
  -0.296003;-0.125781;,
  -0.296003;-0.075351;,
  -0.296003;-0.035008;,
  0.147440;1.034190;,
  0.852560;1.034190;,
  0.852560;1.016850;,
  0.147440;1.016850;,
  0.121940;2.053230;,
  0.878060;2.053230;,
  0.878060;2.093570;,
  0.121940;2.093570;;
 }
}
