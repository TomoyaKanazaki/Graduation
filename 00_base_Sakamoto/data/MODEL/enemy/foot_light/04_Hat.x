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
 22;
 -9.59657;0.01784;-6.97233;,
 -3.66557;0.01784;-11.28145;,
 0.00000;2.58360;0.00000;,
 6.05568;2.91263;4.39970;,
 0.00000;5.73910;0.00000;,
 2.31306;2.91263;7.11887;,
 3.66557;0.01784;11.28144;,
 -2.31306;2.91263;7.11887;,
 -3.66557;0.01784;11.28143;,
 -6.05568;2.91263;4.39970;,
 -9.59657;0.01784;6.97231;,
 -7.48523;2.91263;0.00000;,
 -11.86201;0.01784;0.00000;,
 -6.05568;2.91263;-4.39972;,
 -2.31306;2.91263;-7.11888;,
 2.31306;2.91263;-7.11888;,
 3.66557;0.01784;-11.28145;,
 6.05568;2.91263;-4.39972;,
 9.59657;0.01784;-6.97233;,
 7.48523;2.91263;0.00000;,
 11.86201;0.01784;0.00000;,
 9.59657;0.01784;6.97231;;
 
 30;
 3;0,1,2;,
 3;3,4,5;,
 4;6,5,7,8;,
 4;8,7,9,10;,
 4;10,9,11,12;,
 4;12,11,13,0;,
 4;0,13,14,1;,
 3;14,4,15;,
 4;16,15,17,18;,
 3;17,4,19;,
 4;20,19,3,21;,
 3;10,12,2;,
 3;6,8,2;,
 3;16,18,2;,
 3;21,2,20;,
 3;1,16,2;,
 3;12,0,2;,
 3;21,6,2;,
 3;8,10,2;,
 3;18,20,2;,
 4;1,14,15,16;,
 4;18,17,19,20;,
 4;21,3,5,6;,
 3;7,4,9;,
 3;11,4,13;,
 3;9,4,11;,
 3;5,4,7;,
 3;19,4,3;,
 3;15,4,17;,
 3;13,4,14;;
 
 MeshMaterialList {
  1;
  30;
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
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\enemy\\foot_light_UV.jpg";
   }
  }
 }
 MeshNormals {
  32;
  0.446297;0.834073;0.324254;,
  0.170470;0.834073;0.524654;,
  -0.170471;0.834073;0.524654;,
  -0.446297;0.834073;0.324254;,
  -0.551653;0.834074;0.000000;,
  0.171035;-0.977397;0.124264;,
  0.065330;-0.977397;0.201064;,
  0.170470;0.834074;-0.524653;,
  0.446297;0.834074;-0.324253;,
  0.551653;0.834074;0.000000;,
  -0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.140550;0.890579;-0.432568;,
  0.140550;0.890579;-0.432568;,
  0.367964;0.890579;-0.267342;,
  0.454829;0.890579;0.000000;,
  0.367964;0.890579;0.267342;,
  0.140550;0.890579;0.432568;,
  -0.367964;0.890579;0.267342;,
  -0.140550;0.890579;0.432568;,
  -0.367964;0.890579;-0.267341;,
  -0.454829;0.890579;0.000000;,
  -0.446296;0.834074;-0.324253;,
  -0.170470;0.834074;-0.524653;,
  0.171036;-0.977397;-0.124265;,
  0.211411;-0.977397;-0.000000;,
  -0.065330;-0.977397;-0.201064;,
  0.065330;-0.977397;-0.201064;,
  -0.065330;-0.977397;0.201064;,
  -0.171035;-0.977397;0.124264;,
  -0.171036;-0.977397;-0.124265;,
  -0.211411;-0.977397;-0.000000;;
  30;
  3;5,6,11;,
  3;16,10,17;,
  4;1,17,19,2;,
  4;2,19,18,3;,
  4;3,18,21,4;,
  4;4,21,20,22;,
  4;22,20,12,23;,
  3;12,10,13;,
  4;7,13,14,8;,
  3;14,10,15;,
  4;9,15,16,0;,
  3;24,25,11;,
  3;26,27,11;,
  3;28,29,11;,
  3;30,11,31;,
  3;6,28,11;,
  3;25,5,11;,
  3;30,26,11;,
  3;27,24,11;,
  3;29,31,11;,
  4;23,12,13,7;,
  4;8,14,15,9;,
  4;0,16,17,1;,
  3;19,10,18;,
  3;21,10,20;,
  3;18,10,21;,
  3;17,10,19;,
  3;15,10,16;,
  3;13,10,14;,
  3;20,10,12;;
 }
 MeshTextureCoords {
  22;
  0.690578;0.614614;,
  0.641802;0.614614;,
  0.769499;0.500080;,
  0.819299;0.485393;,
  0.769499;0.359221;,
  0.850078;0.485393;,
  0.897195;0.614614;,
  0.850078;0.485393;,
  0.897195;0.614614;,
  0.819299;0.485393;,
  0.848419;0.614614;,
  0.769499;0.485393;,
  0.769499;0.614614;,
  0.719698;0.485393;,
  0.688919;0.485393;,
  0.688919;0.485393;,
  0.641802;0.614614;,
  0.719698;0.485393;,
  0.690578;0.614614;,
  0.769499;0.485393;,
  0.769499;0.614614;,
  0.848419;0.614614;;
 }
}