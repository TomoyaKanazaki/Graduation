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
 66;
 3.36604;-12.33836;0.67164;,
 2.76339;-12.33836;3.81964;,
 2.34450;-15.36848;3.32585;,
 2.51551;-15.36848;0.97328;,
 0.00000;0.55022;0.00000;,
 0.00000;0.55022;4.44083;,
 2.31469;0.55022;3.93809;,
 2.81948;0.55022;1.30124;,
 -2.81948;0.55022;1.30124;,
 -2.31469;0.55022;3.93809;,
 -2.51551;-15.36848;0.97328;,
 -2.06514;-15.36848;3.32585;,
 -2.76339;-12.33836;3.81964;,
 -3.36604;-12.33836;0.67164;,
 0.00000;-15.80932;-0.14996;,
 0.00000;-15.80932;2.80613;,
 -1.53227;-15.80932;2.47332;,
 -1.86642;-15.80932;0.72779;,
 1.86643;-15.80932;0.72779;,
 1.73955;-15.80932;2.47332;,
 0.00000;-15.36848;3.77440;,
 0.00000;-12.33836;4.41984;,
 -2.34450;-15.36848;-2.46462;,
 -3.13720;-12.33836;-3.92865;,
 0.00000;-12.33836;-5.62266;,
 0.00000;-15.36848;-3.73060;,
 3.13720;-12.33836;-3.92865;,
 2.34450;-15.36848;-2.46462;,
 0.00000;-15.80932;-2.76232;,
 1.73955;-15.80932;-1.82302;,
 -1.73955;-15.80932;-1.82302;,
 0.00000;0.55022;-3.97103;,
 -2.62780;0.55022;-2.55208;,
 2.62780;0.55022;-2.55208;,
 -2.17693;-15.61871;0.84522;,
 -1.78718;-15.61871;2.88114;,
 0.00000;-15.61871;3.26931;,
 2.02893;-15.61871;2.88114;,
 2.17693;-15.61871;0.84522;,
 2.02893;-15.61871;-2.12993;,
 0.00000;-15.61871;-3.22551;,
 -2.02893;-15.61871;-2.12993;,
 4.00015;-6.58863;1.36149;,
 3.28398;-6.58863;5.10253;,
 0.00000;-6.58863;5.81580;,
 -3.28398;-6.58863;5.10253;,
 -4.00015;-6.58863;1.36149;,
 -3.72821;-6.58863;-4.10544;,
 0.00000;-6.58863;-6.11858;,
 3.72822;-6.58863;-4.10544;,
 3.79393;-0.66255;1.80788;,
 3.11468;-0.66255;5.35606;,
 0.00000;-0.66255;6.03256;,
 -3.11467;-0.66255;5.35606;,
 -3.79393;-0.66255;1.80788;,
 -3.53601;-0.66255;-3.37720;,
 0.00000;-0.66255;-5.28656;,
 3.53601;-0.66255;-3.37720;,
 3.32953;0.00000;1.49415;,
 2.73342;0.00000;4.60801;,
 0.00000;0.00000;5.20170;,
 -2.73342;0.00000;4.60801;,
 -3.32953;0.00000;1.49415;,
 -3.10319;0.00000;-3.05625;,
 0.00000;0.00000;-4.73190;,
 3.10319;0.00000;-3.05626;;
 
 64;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,5,4;,
 4;10,11,12,13;,
 4;14,15,16,17;,
 4;18,19,15,14;,
 4;20,21,12,11;,
 4;2,1,21,20;,
 4;22,23,24,25;,
 4;25,24,26,27;,
 4;28,29,18,14;,
 4;30,28,14,17;,
 4;23,22,10,13;,
 4;31,32,8,4;,
 4;33,31,4,7;,
 4;27,26,0,3;,
 4;10,34,35,11;,
 4;11,35,36,20;,
 4;20,36,37,2;,
 4;2,37,38,3;,
 4;3,38,39,27;,
 4;27,39,40,25;,
 4;22,41,34,10;,
 4;25,40,41,22;,
 4;34,17,16,35;,
 4;35,16,15,36;,
 4;36,15,19,37;,
 4;37,19,18,38;,
 4;38,18,29,39;,
 4;39,29,28,40;,
 4;41,30,17,34;,
 4;40,28,30,41;,
 4;0,42,43,1;,
 4;1,43,44,21;,
 4;21,44,45,12;,
 4;12,45,46,13;,
 4;13,46,47,23;,
 4;23,47,48,24;,
 4;26,49,42,0;,
 4;24,48,49,26;,
 4;42,50,51,43;,
 4;43,51,52,44;,
 4;44,52,53,45;,
 4;45,53,54,46;,
 4;46,54,55,47;,
 4;47,55,56,48;,
 4;49,57,50,42;,
 4;48,56,57,49;,
 4;50,58,59,51;,
 4;51,59,60,52;,
 4;52,60,61,53;,
 4;53,61,62,54;,
 4;54,62,63,55;,
 4;55,63,64,56;,
 4;57,65,58,50;,
 4;56,64,65,57;,
 4;58,7,6,59;,
 4;59,6,5,60;,
 4;60,5,9,61;,
 4;61,9,8,62;,
 4;62,8,32,63;,
 4;63,32,31,64;,
 4;65,33,7,58;,
 4;64,31,33,65;;
 
 MeshMaterialList {
  8;
  64;
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
   0.800000;0.800000;0.800000;1.000000;;
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
   0.800000;0.800000;0.800000;1.000000;;
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
   0.800000;0.800000;0.800000;1.000000;;
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
   0.800000;0.800000;0.800000;1.000000;;
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
 }
 MeshNormals {
  74;
  0.627627;-0.688467;-0.363452;,
  0.830824;-0.548998;0.091282;,
  0.820632;-0.269844;-0.503733;,
  0.972387;-0.172245;0.157465;,
  -0.000000;-0.320202;-0.947349;,
  -0.002943;-0.219806;0.975539;,
  -0.820632;-0.269844;-0.503733;,
  -0.964335;-0.189675;0.184610;,
  -0.627627;-0.688467;-0.363452;,
  -0.610277;-0.742099;0.277222;,
  0.000000;-0.747336;-0.664447;,
  -0.006438;-0.614704;0.788731;,
  0.829029;-0.558659;0.024709;,
  -0.819642;-0.570076;0.056578;,
  -0.979197;-0.191000;0.068499;,
  0.981783;-0.181942;0.054765;,
  -0.549334;-0.834796;0.036714;,
  -0.321539;-0.908733;0.266115;,
  -0.005299;-0.910958;0.412465;,
  0.557740;-0.829995;0.005853;,
  0.328843;-0.914273;0.236575;,
  0.000000;-0.911015;-0.412373;,
  0.405541;-0.886481;-0.222909;,
  -0.405540;-0.886481;-0.222909;,
  0.000000;-1.000000;-0.000000;,
  -0.266115;-0.963778;0.017737;,
  -0.200056;-0.965904;0.164341;,
  -0.002491;-0.981064;0.193669;,
  0.270898;-0.962604;0.002827;,
  0.203781;-0.968196;0.145155;,
  0.000000;-0.981403;-0.191958;,
  0.254293;-0.957122;-0.138755;,
  -0.254292;-0.957122;-0.138755;,
  0.996602;-0.044164;0.069523;,
  0.980676;-0.055035;0.187738;,
  -0.000000;-0.136999;0.990571;,
  -0.996602;-0.044164;0.069523;,
  -0.210336;-0.133935;0.968411;,
  -0.000000;0.026799;-0.999641;,
  -0.846134;-0.005251;-0.532945;,
  0.846134;-0.005251;-0.532945;,
  0.943618;0.324653;0.064696;,
  0.925800;0.333891;0.177232;,
  -0.000000;0.420545;0.907272;,
  -0.943618;0.324654;0.064696;,
  -0.453199;0.546589;0.704167;,
  -0.000000;0.412239;-0.911076;,
  -0.783176;0.384230;-0.488878;,
  0.783176;0.384230;-0.488878;,
  0.766212;0.640512;0.051602;,
  0.471834;0.784594;0.402225;,
  -0.000000;0.796316;0.604881;,
  -0.766213;0.640511;0.051603;,
  -0.471835;0.784594;0.402225;,
  -0.000000;0.733055;-0.680169;,
  -0.609716;0.703903;-0.364372;,
  0.609715;0.703904;-0.364372;,
  0.000000;1.000000;0.000000;,
  0.393152;0.919087;0.026653;,
  0.303478;0.916096;0.262048;,
  -0.000000;0.951800;0.306719;,
  -0.393154;0.919086;0.026653;,
  -0.303479;0.916096;0.262048;,
  -0.000000;0.953750;-0.300602;,
  -0.382242;0.897820;-0.218655;,
  0.382241;0.897821;-0.218655;,
  -0.207175;-0.217338;0.953857;,
  -0.207795;-0.203787;0.956709;,
  0.156256;-0.605183;0.780601;,
  0.201432;-0.213000;0.956063;,
  0.210336;-0.133935;0.968411;,
  -0.980676;-0.055035;0.187738;,
  0.193339;0.412610;0.890153;,
  -0.981968;0.020012;0.187985;;
  64;
  4;15,3,1,12;,
  4;57,60,59,58;,
  4;61,62,60,57;,
  4;13,9,7,14;,
  4;24,27,26,25;,
  4;28,29,27,24;,
  4;11,5,66,67;,
  4;68,69,5,11;,
  4;8,6,4,10;,
  4;10,4,2,0;,
  4;30,31,28,24;,
  4;32,30,24,25;,
  4;6,8,13,14;,
  4;63,64,61,57;,
  4;65,63,57,58;,
  4;0,2,15,12;,
  4;13,16,17,9;,
  4;9,17,18,11;,
  4;11,18,20,68;,
  4;1,20,19,12;,
  4;12,19,22,0;,
  4;0,22,21,10;,
  4;8,23,16,13;,
  4;10,21,23,8;,
  4;16,25,26,17;,
  4;17,26,27,18;,
  4;18,27,29,20;,
  4;20,29,28,19;,
  4;19,28,31,22;,
  4;22,31,30,21;,
  4;23,32,25,16;,
  4;21,30,32,23;,
  4;15,33,34,3;,
  4;69,70,35,5;,
  4;5,35,37,66;,
  4;7,71,36,14;,
  4;14,36,39,6;,
  4;6,39,38,4;,
  4;2,40,33,15;,
  4;4,38,40,2;,
  4;33,41,42,34;,
  4;70,72,43,35;,
  4;35,43,45,37;,
  4;71,73,44,36;,
  4;36,44,47,39;,
  4;39,47,46,38;,
  4;40,48,41,33;,
  4;38,46,48,40;,
  4;41,49,50,42;,
  4;72,50,51,43;,
  4;43,51,53,45;,
  4;45,53,52,44;,
  4;44,52,55,47;,
  4;47,55,54,46;,
  4;48,56,49,41;,
  4;46,54,56,48;,
  4;49,58,59,50;,
  4;50,59,60,51;,
  4;51,60,62,53;,
  4;53,62,61,52;,
  4;52,61,64,55;,
  4;55,64,63,54;,
  4;56,65,58,49;,
  4;54,63,65,56;;
 }
 MeshTextureCoords {
  66;
  0.390370;0.235150;,
  0.368520;0.235150;,
  0.382070;0.286850;,
  0.390370;0.286850;,
  0.268320;0.053070;,
  0.268320;0.053070;,
  0.338040;0.053070;,
  0.353240;0.053070;,
  0.183400;0.053070;,
  0.198610;0.053070;,
  0.146270;0.286850;,
  0.168130;0.286850;,
  0.168130;0.235150;,
  0.146270;0.235150;,
  0.268320;0.297160;,
  0.268320;0.297160;,
  0.193980;0.297160;,
  0.177770;0.297160;,
  0.358880;0.297160;,
  0.352720;0.297160;,
  0.268320;0.286850;,
  0.268320;0.235150;,
  0.154570;0.286850;,
  0.154570;0.235150;,
  0.268320;0.235150;,
  0.268320;0.286850;,
  0.382070;0.235150;,
  0.382070;0.286850;,
  0.268320;0.297160;,
  0.352720;0.297160;,
  0.183920;0.297160;,
  0.268320;0.053070;,
  0.189170;0.053070;,
  0.347470;0.053070;,
  0.162700;0.292700;,
  0.181610;0.292700;,
  0.268320;0.292700;,
  0.366760;0.292700;,
  0.373940;0.292700;,
  0.366760;0.292700;,
  0.268320;0.292700;,
  0.169880;0.292700;,
  0.388800;0.156790;,
  0.367230;0.156790;,
  0.268320;0.156790;,
  0.169410;0.156790;,
  0.147840;0.156790;,
  0.156030;0.156790;,
  0.268320;0.156790;,
  0.380610;0.156790;,
  0.382590;0.070690;,
  0.362130;0.070690;,
  0.268320;0.070690;,
  0.174510;0.070690;,
  0.154050;0.070690;,
  0.161820;0.070690;,
  0.268320;0.070690;,
  0.374820;0.070690;,
  0.368600;0.061060;,
  0.350650;0.061060;,
  0.268320;0.061060;,
  0.185990;0.061060;,
  0.168040;0.061060;,
  0.174860;0.061060;,
  0.268320;0.061060;,
  0.361780;0.061060;;
 }
}