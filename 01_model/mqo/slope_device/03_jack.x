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
 172;
 -34.99682;0.38616;55.80784;,
 -34.99682;68.98886;55.80784;,
 -26.92601;68.98886;55.80784;,
 -26.92601;0.38616;55.80784;,
 33.20842;68.98882;55.80784;,
 33.20842;0.38616;55.80784;,
 25.13761;0.38616;55.80784;,
 25.13761;68.98886;55.80784;,
 37.16339;68.98882;65.67140;,
 33.20842;68.98882;75.53488;,
 33.20842;0.38616;75.53488;,
 37.16339;0.38616;65.67140;,
 -38.95179;68.98886;65.67140;,
 -38.95179;0.38616;65.67140;,
 -34.99682;0.38616;75.53488;,
 -34.99682;68.98886;75.53488;,
 12.12170;103.33049;55.80784;,
 12.12170;68.98886;55.80784;,
 25.13761;103.33049;55.80784;,
 12.12168;84.58485;-84.64227;,
 25.13759;84.58485;-84.64227;,
 25.13759;68.98886;-84.64227;,
 12.12168;68.98886;-84.64227;,
 25.13759;0.38616;-84.64227;,
 12.12168;0.38616;-84.64227;,
 -13.91011;103.33049;55.80784;,
 -26.92601;103.33049;55.80784;,
 -13.91011;68.98886;55.80784;,
 -13.91013;84.58485;-84.64227;,
 -13.91013;68.98886;-84.64227;,
 -26.92604;68.98886;-84.64227;,
 -26.92604;84.58485;-84.64227;,
 -13.91013;0.38616;-84.64227;,
 -26.92604;0.38616;-84.64227;,
 -13.91012;103.33049;17.50732;,
 -26.92602;103.33049;17.50732;,
 -13.91011;103.33049;55.80784;,
 -26.92602;103.33049;17.50732;,
 -26.92602;68.98886;17.50732;,
 -26.92601;68.98886;55.80784;,
 -26.92601;103.33049;55.80784;,
 12.12169;103.33049;17.50732;,
 12.12170;103.33049;55.80784;,
 25.13761;103.33049;55.80784;,
 25.13760;103.33049;17.50732;,
 -26.92602;0.38616;17.50732;,
 -26.92601;0.38616;55.80784;,
 25.13760;103.33049;17.50732;,
 25.13761;103.33049;55.80784;,
 25.13761;68.98886;55.80784;,
 25.13760;68.98886;17.50732;,
 -26.92602;0.38616;17.50732;,
 -13.91012;0.38616;17.50732;,
 -13.91011;0.38616;55.80784;,
 25.13761;0.38616;55.80784;,
 25.13760;0.38616;17.50732;,
 25.13760;0.38616;17.50732;,
 25.13761;0.38616;55.80784;,
 12.12170;0.38616;55.80784;,
 12.12169;0.38616;17.50732;,
 12.12170;0.38616;55.80784;,
 -13.91011;0.38616;55.80784;,
 12.12171;68.98886;65.67140;,
 -13.91010;68.98886;65.67140;,
 12.12171;68.98886;79.18290;,
 -13.91010;68.98886;79.18290;,
 12.12171;0.38616;79.18290;,
 -13.91010;0.38616;79.18290;,
 12.12171;0.38616;65.67140;,
 -13.91010;0.38616;65.67140;,
 25.13759;103.33049;-19.04365;,
 25.13759;68.98886;-19.04365;,
 25.13759;68.98886;-84.64227;,
 25.13759;84.58485;-84.64227;,
 25.13759;103.33049;-19.04365;,
 25.13759;84.58485;-84.64227;,
 12.12168;84.58485;-84.64227;,
 12.12169;103.33049;-19.04365;,
 25.13759;0.38616;-19.04365;,
 25.13759;0.38616;-84.64227;,
 -13.91012;103.33049;-19.04365;,
 25.13759;0.38616;-19.04365;,
 12.12169;0.38616;-19.04365;,
 12.12168;0.38616;-84.64227;,
 25.13759;0.38616;-84.64227;,
 -13.91013;84.58485;-84.64227;,
 -26.92603;103.33049;-19.04365;,
 -13.91012;0.38616;-19.04365;,
 -26.92603;103.33049;-19.04365;,
 -26.92604;84.58485;-84.64227;,
 -26.92604;68.98886;-84.64227;,
 -26.92603;68.98886;-19.04365;,
 -26.92603;0.38616;-19.04365;,
 -13.91013;0.38616;-84.64227;,
 -26.92604;0.38616;-84.64227;,
 -26.92603;0.38616;-19.04365;,
 12.12168;70.62501;-77.42832;,
 12.12169;89.77982;-14.09272;,
 -13.91012;89.77982;-14.09272;,
 -13.91013;70.62501;-77.42832;,
 -22.32445;95.03418;38.59885;,
 -22.32445;91.22235;47.80110;,
 20.53605;91.22235;47.80110;,
 20.53605;95.03418;38.59885;,
 -22.32445;82.01976;51.61292;,
 20.53605;82.01976;51.61292;,
 -22.32445;72.81723;47.80110;,
 20.53605;72.81723;47.80110;,
 -22.32445;69.00542;38.59885;,
 20.53605;69.00542;38.59885;,
 -22.32447;72.81723;29.39620;,
 20.53604;72.81723;29.39620;,
 -22.32447;82.01976;25.58428;,
 20.53604;82.01976;25.58428;,
 -22.32447;91.22235;29.39620;,
 20.53604;91.22235;29.39620;,
 -22.32445;95.03418;38.59885;,
 20.53605;95.03418;38.59885;,
 -22.32445;82.01976;38.59885;,
 -22.32445;82.01976;38.59885;,
 -22.32445;82.01976;38.59885;,
 -22.32445;82.01976;38.59885;,
 -22.32445;82.01976;38.59885;,
 -22.32445;82.01976;38.59885;,
 -22.32445;82.01976;38.59885;,
 -22.32445;82.01976;38.59885;,
 20.53605;82.01976;38.59885;,
 20.53605;82.01976;38.59885;,
 20.53605;82.01976;38.59885;,
 20.53605;82.01976;38.59885;,
 20.53605;82.01976;38.59885;,
 20.53605;82.01976;38.59885;,
 20.53605;82.01976;38.59885;,
 20.53605;82.01976;38.59885;,
 12.44877;80.90302;-18.83173;,
 12.44877;84.20151;-10.86849;,
 -14.23719;84.20151;-10.86849;,
 -14.23719;80.90302;-18.83173;,
 12.44877;92.16479;-7.56995;,
 -14.23719;92.16479;-7.56995;,
 12.44877;100.12798;-10.86849;,
 -14.23719;100.12798;-10.86849;,
 12.44877;103.42652;-18.83173;,
 -14.23719;103.42652;-18.83173;,
 12.44877;100.12798;-26.79507;,
 -14.23719;100.12798;-26.79507;,
 12.44877;92.16479;-33.91807;,
 -14.23720;92.16479;-33.91807;,
 12.44877;80.24910;-41.59683;,
 -14.23720;80.24910;-41.59683;,
 12.44877;80.90302;-18.83173;,
 -14.23719;80.90302;-18.83173;,
 12.44877;92.16479;-18.83173;,
 12.44877;92.16479;-18.83173;,
 12.44877;92.16479;-18.83173;,
 12.44877;92.16479;-18.83173;,
 12.44877;92.16479;-18.83173;,
 12.44877;92.16479;-18.83173;,
 12.44877;92.16479;-18.83173;,
 12.44877;92.16479;-18.83173;,
 -14.23719;92.16479;-18.83173;,
 -14.23719;92.16479;-18.83173;,
 -14.23719;92.16479;-18.83173;,
 -14.23719;92.16479;-18.83173;,
 -14.23719;92.16479;-18.83173;,
 -14.23719;92.16479;-18.83173;,
 -14.23719;92.16479;-18.83173;,
 -14.23719;92.16479;-18.83173;,
 12.12168;0.38616;-79.81427;,
 12.12168;70.62501;-77.42832;,
 -13.91013;70.62501;-77.42832;,
 -13.91013;0.38616;-79.81427;;
 
 116;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;8,11,5,4;,
 4;12,1,0,13;,
 4;12,13,14,15;,
 4;16,17,7,18;,
 4;19,20,21,22;,
 4;22,21,23,24;,
 4;25,26,2,27;,
 4;28,29,30,31;,
 4;29,32,33,30;,
 4;34,35,26,36;,
 4;37,38,39,40;,
 4;41,42,43,44;,
 4;38,45,46,39;,
 4;47,48,49,50;,
 4;51,52,53,3;,
 4;50,49,54,55;,
 4;56,57,58,59;,
 4;59,60,16,41;,
 4;34,25,61,52;,
 4;41,34,52,59;,
 4;59,52,27,17;,
 4;62,17,27,63;,
 4;64,62,63,65;,
 4;66,64,65,67;,
 4;68,66,67,69;,
 4;60,68,69,61;,
 4;53,52,59,58;,
 4;10,9,64,66;,
 4;8,4,17,62;,
 4;9,8,62,64;,
 4;11,10,66,68;,
 4;5,11,68,60;,
 4;15,14,67,65;,
 4;1,12,63,27;,
 4;12,15,65,63;,
 4;14,13,69,67;,
 4;13,0,61,69;,
 4;70,47,50,71;,
 4;70,71,72,73;,
 4;74,75,76,77;,
 4;74,77,41,44;,
 4;71,50,55,78;,
 4;71,78,79,72;,
 4;77,80,34,41;,
 4;81,56,59,82;,
 4;81,82,83,84;,
 4;80,85,31,86;,
 4;80,86,35,34;,
 4;82,59,52,87;,
 4;88,89,90,91;,
 4;88,91,38,37;,
 4;87,52,51,92;,
 4;87,92,33,93;,
 4;91,90,94,95;,
 4;91,95,45,38;,
 4;96,97,77,76;,
 4;98,99,85,80;,
 4;97,98,80,77;,
 4;97,96,99,98;,
 3;76,22,96;,
 3;99,29,85;,
 4;100,101,102,103;,
 4;101,104,105,102;,
 4;104,106,107,105;,
 4;106,108,109,107;,
 4;108,110,111,109;,
 4;110,112,113,111;,
 4;112,114,115,113;,
 4;114,116,117,115;,
 3;118,101,100;,
 3;119,104,101;,
 3;120,106,104;,
 3;121,108,106;,
 3;122,110,108;,
 3;123,112,110;,
 3;124,114,112;,
 3;125,116,114;,
 3;126,103,102;,
 3;127,102,105;,
 3;128,105,107;,
 3;129,107,109;,
 3;130,109,111;,
 3;131,111,113;,
 3;132,113,115;,
 3;133,115,117;,
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
 4;168,169,22,24;,
 4;170,171,32,29;,
 4;169,168,171,170;,
 4;171,168,82,87;;
 
 MeshMaterialList {
  5;
  116;
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
  3,
  3,
  3,
  3;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\Material\\SlopeDevice\\dodai001.jpg";
   }
  }
  Material {
   0.800000;0.790588;0.765490;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\Material\\SlopeDevice\\Iron000.png";
   }
  }
  Material {
   0.743529;0.263529;0.194510;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.000000;0.059608;1.000000;;
   5.000000;
   0.100000;0.100000;0.100000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.110000;0.110000;0.110000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  82;
  0.000000;0.000000;-1.000000;,
  0.000000;-0.000000;1.000000;,
  0.629088;0.000000;0.777334;,
  -0.629088;0.000000;0.777334;,
  1.000000;0.000000;0.000001;,
  -1.000000;0.000000;0.000001;,
  0.000001;1.000000;0.000000;,
  0.000001;1.000000;0.000000;,
  0.085548;0.000000;0.996334;,
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;0.000000;,
  -0.085548;0.000000;0.996334;,
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;-0.000000;,
  1.000000;0.000000;-0.000000;,
  1.000000;0.000000;-0.000000;,
  0.000000;0.995724;-0.092382;,
  0.000000;0.990331;-0.138723;,
  -1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  1.000000;-0.000000;-0.000000;,
  -1.000000;0.000000;0.000001;,
  0.000000;1.000000;0.000008;,
  0.000000;0.707102;0.707112;,
  0.000000;-0.000001;1.000000;,
  0.000000;-0.707103;0.707110;,
  0.000000;-1.000000;0.000008;,
  0.000000;-0.707112;-0.707102;,
  0.000000;-0.000001;-1.000000;,
  0.000000;0.707110;-0.707103;,
  1.000000;-0.000000;-0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;-0.977883;0.209152;,
  0.000000;-0.707108;0.707106;,
  0.000000;0.000002;1.000000;,
  0.000000;0.707108;0.707106;,
  0.000000;1.000000;0.000002;,
  0.000000;0.815695;-0.578482;,
  0.000000;0.606065;-0.795415;,
  0.000000;0.541689;-0.840579;,
  -1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  1.000000;-0.000000;-0.000000;,
  0.928167;0.000000;-0.372165;,
  -0.928167;0.000000;-0.372165;,
  -1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;-0.000000;,
  1.000000;0.000000;-0.000000;,
  1.000000;0.000000;-0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.487469;-0.873140;,
  0.000001;1.000000;-0.000000;,
  0.000002;1.000000;0.000000;,
  0.000002;1.000000;-0.000000;,
  0.000002;1.000000;0.000000;,
  0.000000;0.961512;-0.274764;,
  0.000000;0.961512;-0.274764;,
  -1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  1.000000;-0.000000;-0.000000;,
  0.000000;-0.343176;-0.939271;,
  0.000000;0.957183;-0.289484;,
  -1.000000;0.000000;0.000001;,
  -1.000000;0.000000;0.000001;,
  -1.000000;0.000000;0.000002;,
  -1.000000;0.000000;0.000002;,
  -1.000000;-0.000000;0.000002;,
  1.000000;0.000000;-0.000001;,
  1.000000;-0.000000;-0.000001;,
  1.000000;-0.000000;-0.000001;,
  1.000000;0.000000;-0.000001;,
  0.000000;-0.999588;0.028713;,
  -1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000001;,
  -1.000000;-0.000000;0.000001;,
  0.000000;0.033950;-0.999424;;
  116;
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;4,2,2,4;,
  4;4,4,44,44;,
  4;5,45,45,5;,
  4;5,5,3,3;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;10,10,10,10;,
  4;46,12,47,48;,
  4;10,10,10,10;,
  4;12,12,47,47;,
  4;49,50,51,13;,
  4;9,9,9,9;,
  4;13,51,51,13;,
  4;9,9,9,9;,
  4;52,52,52,52;,
  4;51,51,51,51;,
  4;1,1,1,1;,
  4;53,53,53,53;,
  4;6,54,10,10;,
  4;7,6,10,10;,
  4;8,8,11,11;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;2,2,8,8;,
  4;55,56,54,6;,
  4;57,55,6,7;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;3,3,11,11;,
  4;10,10,10,10;,
  4;10,10,10,10;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;14,49,13,15;,
  4;14,15,31,31;,
  4;17,58,58,16;,
  4;17,16,10,10;,
  4;15,13,13,15;,
  4;15,15,31,31;,
  4;16,16,10,10;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;16,59,59,17;,
  4;16,17,10,10;,
  4;9,9,9,9;,
  4;60,61,61,18;,
  4;60,18,12,46;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;18,61,62,18;,
  4;18,18,12,12;,
  4;41,19,19,63;,
  4;20,43,64,20;,
  4;65,65,65,65;,
  4;66,66,66,66;,
  3;63,42,41;,
  3;43,31,64;,
  4;22,23,23,22;,
  4;23,24,24,23;,
  4;24,25,25,24;,
  4;25,26,26,25;,
  4;26,27,27,26;,
  4;27,28,28,27;,
  4;28,29,29,28;,
  4;29,22,22,29;,
  3;21,42,67;,
  3;21,42,42;,
  3;21,42,42;,
  3;21,68,42;,
  3;21,69,68;,
  3;21,70,69;,
  3;21,71,70;,
  3;21,67,71;,
  3;30,72,31;,
  3;30,31,31;,
  3;30,31,31;,
  3;30,31,72;,
  3;30,72,73;,
  3;30,73,74;,
  3;30,74,75;,
  3;30,75,72;,
  4;32,33,33,32;,
  4;33,34,34,33;,
  4;34,35,35,34;,
  4;35,36,36,35;,
  4;36,37,37,36;,
  4;37,38,38,37;,
  4;38,39,39,38;,
  4;76,32,32,76;,
  3;31,31,31;,
  3;31,31,31;,
  3;31,31,31;,
  3;31,31,31;,
  3;31,31,31;,
  3;31,31,31;,
  3;31,31,31;,
  3;31,31,31;,
  3;40,77,42;,
  3;40,42,42;,
  3;40,42,42;,
  3;40,42,42;,
  3;40,42,78;,
  3;40,78,79;,
  3;40,79,80;,
  3;40,80,77;,
  4;42,41,42,42;,
  4;43,31,31,31;,
  4;81,81,81,81;,
  4;9,9,9,9;;
 }
 MeshTextureCoords {
  172;
  1.000000;1.000000;,
  1.000000;0.898630;,
  1.000000;0.898630;,
  1.000000;1.000000;,
  0.000000;0.898630;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.898630;,
  0.000000;0.898630;,
  0.000000;0.898630;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.898630;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.898630;,
  0.250000;0.000000;,
  0.250000;0.898630;,
  0.000000;0.000000;,
  0.750000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.898630;,
  0.750000;0.898630;,
  1.000000;1.000000;,
  0.750000;1.000000;,
  0.750000;0.000000;,
  1.000000;0.000000;,
  0.750000;0.898630;,
  0.250000;0.000000;,
  0.250000;0.898630;,
  0.000000;0.898630;,
  0.000000;0.000000;,
  0.250000;1.000000;,
  0.000000;1.000000;,
  0.687600;0.250000;,
  0.687600;0.000000;,
  1.000000;0.250000;,
  0.312400;0.000000;,
  0.312400;0.898630;,
  0.000000;0.898630;,
  0.000000;0.000000;,
  0.687600;0.750000;,
  1.000000;0.750000;,
  1.000000;1.000000;,
  0.687600;1.000000;,
  0.312400;1.000000;,
  0.000000;1.000000;,
  0.687600;0.000000;,
  1.000000;0.000000;,
  1.000000;0.898630;,
  0.687600;0.898630;,
  0.687600;1.000000;,
  0.687600;0.750000;,
  1.000000;0.750000;,
  1.000000;1.000000;,
  0.687600;1.000000;,
  0.687600;0.000000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  0.687600;0.250000;,
  0.250000;1.000000;,
  0.750000;1.000000;,
  0.250000;0.898630;,
  0.750000;0.898630;,
  0.250000;0.898630;,
  0.750000;0.898630;,
  0.250000;1.000000;,
  0.750000;1.000000;,
  0.250000;1.000000;,
  0.750000;1.000000;,
  0.445900;0.000000;,
  0.445900;0.898630;,
  0.000000;0.898630;,
  0.000000;0.000000;,
  0.445900;1.000000;,
  0.000000;1.000000;,
  0.000000;0.750000;,
  0.445900;0.750000;,
  0.445900;1.000000;,
  0.000000;1.000000;,
  0.445900;0.250000;,
  0.445900;0.000000;,
  0.445900;0.250000;,
  0.000000;0.250000;,
  0.000000;0.000000;,
  0.000000;0.250000;,
  0.445900;0.000000;,
  0.445900;0.750000;,
  0.554100;0.000000;,
  1.000000;0.000000;,
  1.000000;0.898630;,
  0.554100;0.898630;,
  0.445900;1.000000;,
  0.000000;0.750000;,
  1.000000;1.000000;,
  0.554100;1.000000;,
  0.000000;0.750000;,
  0.445900;0.750000;,
  0.445900;0.250000;,
  0.000000;0.250000;,
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
  0.750000;1.000000;,
  0.750000;0.898630;,
  0.250000;0.898630;,
  0.250000;1.000000;;
 }
}