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
 16;
 2.47270;1.06847;0.79335;,
 2.47151;1.36256;0.03434;,
 -0.52204;1.33435;0.03760;,
 -0.52098;1.05113;0.76850;,
 -0.51692;0.00420;1.02349;,
 2.47690;-0.01878;1.01870;,
 2.47270;1.06847;-0.72463;,
 -0.52098;1.05113;-0.69328;,
 -0.51281;-1.04277;0.76850;,
 2.48116;-1.10601;0.79335;,
 2.47690;-0.01878;-1.01212;,
 -0.51692;0.00415;-0.94827;,
 -0.51169;-1.32598;0.03760;,
 2.48232;-1.40007;0.03434;,
 2.48116;-1.10601;-0.72463;,
 -0.51281;-1.04277;-0.69328;;
 
 14;
 4;0,1,2,3;,
 4;0,3,4,5;,
 4;1,6,7,2;,
 4;5,4,8,9;,
 4;6,10,11,7;,
 4;9,8,12,13;,
 4;10,14,15,11;,
 4;13,12,15,14;,
 4;11,15,12,8;,
 4;7,11,8,4;,
 4;7,4,3,2;,
 4;9,13,14,10;,
 4;10,0,5,9;,
 4;6,1,0,10;;
 
 MeshMaterialList {
  12;
  14;
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.228800;0.295200;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.615200;0.615200;0.615200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.000000;0.050400;1.000000;;
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
  Material {
   0.599200;0.599200;0.599200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.009600;0.156800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.080000;0.080000;0.080000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.339200;0.000000;0.060000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.056800;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.392000;0.000000;0.021600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.323200;0.263200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  18;
  -0.006544;0.652854;0.757455;,
  -0.013662;0.662992;-0.748502;,
  -0.004865;-0.000016;0.999988;,
  -0.017607;-0.000061;-0.999845;,
  -0.016443;-0.652780;0.757369;,
  -0.023905;-0.662913;-0.748315;,
  -0.024744;-0.999694;-0.000005;,
  -0.009425;0.999956;-0.000008;,
  -0.999992;-0.003915;0.000000;,
  -0.999992;-0.003927;0.000000;,
  -0.999992;-0.003882;-0.000006;,
  -0.999993;-0.003862;-0.000012;,
  0.999992;0.003911;0.000023;,
  0.999992;0.003931;0.000000;,
  0.999992;0.003924;0.000015;,
  0.999992;0.003920;0.000023;,
  0.999992;0.003891;0.000045;,
  0.999992;0.003949;0.000000;;
  14;
  4;0,7,7,0;,
  4;0,0,2,2;,
  4;7,1,1,7;,
  4;2,2,4,4;,
  4;1,3,3,1;,
  4;4,4,6,6;,
  4;3,5,5,3;,
  4;6,6,5,5;,
  4;8,9,9,8;,
  4;10,8,8,10;,
  4;10,10,11,11;,
  4;12,13,13,14;,
  4;14,15,16,12;,
  4;17,17,15,14;;
 }
 MeshTextureCoords {
  16;
  0.750000;1.000000;,
  0.875000;1.000000;,
  0.875000;1.000000;,
  0.750000;1.000000;,
  0.750000;0.500000;,
  0.750000;0.500000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.750000;0.000000;,
  0.750000;0.000000;,
  1.000000;0.500000;,
  1.000000;0.500000;,
  0.875000;0.000000;,
  0.875000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;;
 }
}