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
 17;
 -0.94258;-1.80920;1.32991;,
 3.72836;-1.16335;0.79191;,
 3.73354;-0.10180;1.05039;,
 -0.93368;0.02298;1.77614;,
 -0.94258;-1.80920;-1.22818;,
 -0.93371;0.02303;-1.67442;,
 3.73354;-0.10180;-0.94874;,
 3.72836;-1.16335;-0.69020;,
 3.73858;0.95968;0.79191;,
 -0.92484;1.85516;1.32991;,
 -0.92484;1.85516;-1.22818;,
 3.73858;0.95968;-0.69020;,
 3.74006;1.24682;0.05085;,
 3.73354;-0.10180;0.05085;,
 -0.92251;2.35069;0.05085;,
 3.72704;-1.45050;0.05085;,
 -0.94503;-2.30480;0.05085;;
 
 15;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;3,2,8,9;,
 4;5,10,11,6;,
 4;12,13,6,11;,
 4;12,8,2,13;,
 4;12,14,9,8;,
 4;12,11,10,14;,
 4;13,15,7,6;,
 4;13,2,1,15;,
 4;15,16,4,7;,
 4;15,1,0,16;,
 4;9,14,10,5;,
 4;5,0,3,9;,
 4;4,16,0,5;;
 
 MeshMaterialList {
  12;
  15;
  4,
  4,
  4,
  4,
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
  23;
  0.175002;-0.648811;-0.740553;,
  0.208416;0.642294;-0.737578;,
  0.153614;-0.000919;-0.988130;,
  0.208424;0.642282;0.737586;,
  0.153623;-0.000922;0.988129;,
  0.175010;-0.648802;0.740559;,
  0.999989;-0.004783;0.000013;,
  0.999988;-0.004823;0.000007;,
  0.230394;0.973097;-0.000001;,
  0.999988;-0.004863;-0.000000;,
  0.179875;-0.983690;-0.000001;,
  0.999988;-0.004828;-0.000066;,
  0.999988;-0.004794;-0.000066;,
  0.999989;-0.004772;0.000092;,
  0.999988;-0.004817;0.000079;,
  0.999988;-0.004863;-0.000066;,
  0.999988;-0.004863;0.000066;,
  -0.999988;0.004832;0.000013;,
  -0.999988;0.004822;-0.000014;,
  -0.999988;0.004840;0.000005;,
  -0.999988;0.004850;0.000015;,
  -0.999988;0.004843;0.000039;,
  -0.999988;0.004857;-0.000010;;
  15;
  4;5,5,4,4;,
  4;0,2,2,0;,
  4;4,4,3,3;,
  4;2,1,1,2;,
  4;6,7,11,12;,
  4;6,13,14,7;,
  4;8,8,3,3;,
  4;8,1,1,8;,
  4;7,9,15,11;,
  4;7,14,16,9;,
  4;10,10,0,0;,
  4;10,5,5,10;,
  4;17,18,18,19;,
  4;19,20,21,17;,
  4;22,22,20,19;;
 }
 MeshTextureCoords {
  17;
  0.750000;1.000000;,
  0.750000;1.000000;,
  0.750000;0.500000;,
  0.750000;0.500000;,
  1.000000;1.000000;,
  1.000000;0.500000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.750000;0.000000;,
  0.750000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  0.875000;0.000000;,
  0.875000;0.500000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  0.875000;1.000000;;
 }
}
