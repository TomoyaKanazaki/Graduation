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
 158;
 0.00000;4.97024;0.00000;,
 2.09824;4.54977;-2.09824;,
 0.00000;4.54977;-2.96736;,
 0.00000;4.97024;0.00000;,
 2.96736;4.54977;0.00000;,
 0.00000;4.97024;0.00000;,
 2.09824;4.54977;2.09824;,
 0.00000;4.97024;0.00000;,
 0.00000;4.54977;2.96736;,
 0.00000;4.97024;0.00000;,
 -2.09824;4.54977;2.09824;,
 0.00000;4.97024;0.00000;,
 -2.96736;4.54977;0.00000;,
 0.00000;4.97024;0.00000;,
 -2.09824;4.54977;-2.09824;,
 0.00000;4.97024;0.00000;,
 0.00000;4.54977;-2.96736;,
 3.87704;3.35236;-3.87704;,
 0.00000;3.35236;-5.48296;,
 5.48296;3.35236;0.00000;,
 3.87704;3.35236;3.87704;,
 0.00000;3.35236;5.48296;,
 -3.87704;3.35236;3.87704;,
 -5.48296;3.35236;0.00000;,
 -3.87704;3.35236;-3.87704;,
 0.00000;3.35236;-5.48296;,
 5.06560;1.56031;-5.06560;,
 0.00000;1.56031;-7.16384;,
 7.16384;1.56031;0.00000;,
 5.06560;1.56031;5.06560;,
 0.00000;1.56031;7.16384;,
 -5.06560;1.56031;5.06560;,
 -7.16384;1.56031;0.00000;,
 -5.06560;1.56031;-5.06560;,
 0.00000;1.56031;-7.16384;,
 5.48296;0.40640;-5.48296;,
 0.00000;0.40640;-7.75408;,
 7.75408;0.40640;0.00000;,
 5.48296;0.40640;5.48296;,
 0.00000;0.40640;7.75408;,
 -5.48296;0.40640;5.48296;,
 -7.75408;0.40640;0.00000;,
 -5.48296;0.40640;-5.48296;,
 0.00000;0.40640;-7.75408;,
 5.06560;0.10661;-5.06560;,
 0.00000;0.10661;-7.16384;,
 7.16384;0.10661;0.00000;,
 5.06560;0.10661;5.06560;,
 0.00000;0.10661;7.16384;,
 -5.06560;0.10661;5.06560;,
 -7.16384;0.10661;0.00000;,
 -5.06560;0.10661;-5.06560;,
 0.00000;0.10661;-7.16384;,
 3.87704;-0.27143;-3.87704;,
 0.00000;-0.27143;-5.48296;,
 5.48296;-0.27143;0.00000;,
 3.87704;-0.27143;3.87704;,
 -0.00000;-0.27143;5.48296;,
 -3.87704;-0.27143;3.87704;,
 -5.48296;-0.27143;0.00000;,
 -3.87704;-0.27143;-3.87704;,
 0.00000;-0.27143;-5.48296;,
 2.09824;-0.52403;-2.09824;,
 0.00000;-0.52403;-2.96736;,
 2.96736;-0.52403;0.00000;,
 2.09824;-0.52403;2.09824;,
 -0.00000;-0.52403;2.96736;,
 -2.09824;-0.52403;2.09824;,
 -2.96736;-0.52403;0.00000;,
 -2.09824;-0.52403;-2.09824;,
 0.00000;-0.52403;-2.96736;,
 0.00000;-0.61273;-0.00000;,
 0.00000;-0.61273;-0.00000;,
 0.00000;-0.61273;-0.00000;,
 0.00000;-0.61273;-0.00000;,
 0.00000;-0.61273;-0.00000;,
 0.00000;-0.61273;-0.00000;,
 0.00000;-0.61273;-0.00000;,
 0.00000;-0.61273;-0.00000;,
 0.00000;0.57967;0.00000;,
 5.80406;0.53554;-5.80406;,
 0.00000;0.53554;-8.20818;,
 0.00000;0.57967;0.00000;,
 8.20818;0.53554;0.00000;,
 0.00000;0.57967;0.00000;,
 5.80406;0.53554;5.80406;,
 0.00000;0.57967;0.00000;,
 0.00000;0.53554;8.20818;,
 0.00000;0.57967;0.00000;,
 -5.80406;0.53554;5.80406;,
 0.00000;0.57967;0.00000;,
 -8.20818;0.53554;0.00000;,
 0.00000;0.57967;0.00000;,
 -5.80406;0.53554;-5.80406;,
 0.00000;0.57967;0.00000;,
 0.00000;0.53554;-8.20818;,
 10.72451;0.40989;-10.72451;,
 0.00000;0.40989;-15.16675;,
 15.16675;0.40989;0.00000;,
 10.72451;0.40989;10.72451;,
 -7.81389;0.83223;12.52494;,
 -10.72451;0.40989;10.72451;,
 -15.16675;-1.04168;0.00000;,
 -10.72451;0.40989;-10.72451;,
 0.00000;0.40989;-15.16675;,
 14.01224;0.22183;-14.01224;,
 2.81915;-0.04145;-19.56923;,
 19.82037;0.22073;0.00000;,
 20.63651;0.45040;10.18084;,
 -7.81389;0.45040;21.96540;,
 -18.09375;0.22962;10.84106;,
 -23.25270;-0.65376;-6.65888;,
 -17.84943;0.11246;-16.91583;,
 2.81915;-0.04145;-19.56923;,
 15.68277;-0.00341;-14.80852;,
 2.30313;-0.25987;-21.56017;,
 21.44496;-0.00000;0.00000;,
 22.98064;-0.00000;12.52494;,
 -7.81389;0.00000;25.28042;,
 -19.24826;0.00780;11.99556;,
 -24.88541;-0.87559;-6.65888;,
 -19.00394;-0.10937;-18.07033;,
 2.30313;-0.25987;-21.56017;,
 14.01224;-0.22183;-14.01224;,
 2.81915;-0.48511;-19.56923;,
 19.82037;-0.22073;0.00000;,
 20.63651;-0.45040;10.18084;,
 -7.81389;-0.45040;21.96540;,
 -18.09375;-0.21403;10.84106;,
 -23.25270;-1.09742;-6.65888;,
 -17.84943;-0.33119;-16.91583;,
 2.81915;-0.48511;-19.56923;,
 10.72451;-0.40989;-10.72451;,
 0.00000;-0.40989;-15.16675;,
 15.16675;-0.40989;0.00000;,
 10.72451;-0.40989;10.72451;,
 -7.81389;-0.83223;12.52494;,
 -10.72451;-0.40989;10.72451;,
 -15.16675;-1.86146;0.00000;,
 -10.72451;-0.40989;-10.72451;,
 0.00000;-0.40989;-15.16675;,
 5.80406;-0.53554;-5.80406;,
 0.00000;-0.53554;-8.20818;,
 8.20818;-0.53554;0.00000;,
 5.80406;-0.53554;5.80406;,
 -0.00000;-0.53554;8.20818;,
 -5.80406;-0.53554;5.80406;,
 -8.20818;-0.53554;0.00000;,
 -5.80406;-0.53554;-5.80406;,
 0.00000;-0.53554;-8.20818;,
 0.00000;-0.57967;-0.00000;,
 0.00000;-0.57967;-0.00000;,
 0.00000;-0.57967;-0.00000;,
 0.00000;-0.57967;-0.00000;,
 0.00000;-0.57967;-0.00000;,
 0.00000;-0.57967;-0.00000;,
 0.00000;-0.57967;-0.00000;,
 0.00000;-0.57967;-0.00000;;
 
 128;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 3;11,12,10;,
 3;13,14,12;,
 3;15,16,14;,
 4;2,1,17,18;,
 4;1,4,19,17;,
 4;4,6,20,19;,
 4;6,8,21,20;,
 4;8,10,22,21;,
 4;10,12,23,22;,
 4;12,14,24,23;,
 4;14,16,25,24;,
 4;18,17,26,27;,
 4;17,19,28,26;,
 4;19,20,29,28;,
 4;20,21,30,29;,
 4;21,22,31,30;,
 4;22,23,32,31;,
 4;23,24,33,32;,
 4;24,25,34,33;,
 4;27,26,35,36;,
 4;26,28,37,35;,
 4;28,29,38,37;,
 4;29,30,39,38;,
 4;30,31,40,39;,
 4;31,32,41,40;,
 4;32,33,42,41;,
 4;33,34,43,42;,
 4;36,35,44,45;,
 4;35,37,46,44;,
 4;37,38,47,46;,
 4;38,39,48,47;,
 4;39,40,49,48;,
 4;40,41,50,49;,
 4;41,42,51,50;,
 4;42,43,52,51;,
 4;45,44,53,54;,
 4;44,46,55,53;,
 4;46,47,56,55;,
 4;47,48,57,56;,
 4;48,49,58,57;,
 4;49,50,59,58;,
 4;50,51,60,59;,
 4;51,52,61,60;,
 4;54,53,62,63;,
 4;53,55,64,62;,
 4;55,56,65,64;,
 4;56,57,66,65;,
 4;57,58,67,66;,
 4;58,59,68,67;,
 4;59,60,69,68;,
 4;60,61,70,69;,
 3;63,62,71;,
 3;62,64,72;,
 3;64,65,73;,
 3;65,66,74;,
 3;66,67,75;,
 3;67,68,76;,
 3;68,69,77;,
 3;69,70,78;,
 3;79,80,81;,
 3;82,83,80;,
 3;84,85,83;,
 3;86,87,85;,
 3;88,89,87;,
 3;90,91,89;,
 3;92,93,91;,
 3;94,95,93;,
 4;81,80,96,97;,
 4;80,83,98,96;,
 4;83,85,99,98;,
 4;85,87,100,99;,
 4;87,89,101,100;,
 4;89,91,102,101;,
 4;91,93,103,102;,
 4;93,95,104,103;,
 4;97,96,105,106;,
 4;96,98,107,105;,
 4;98,99,108,107;,
 4;99,100,109,108;,
 4;100,101,110,109;,
 4;101,102,111,110;,
 4;102,103,112,111;,
 4;103,104,113,112;,
 4;106,105,114,115;,
 4;105,107,116,114;,
 4;107,108,117,116;,
 4;108,109,118,117;,
 4;109,110,119,118;,
 4;110,111,120,119;,
 4;111,112,121,120;,
 4;112,113,122,121;,
 4;115,114,123,124;,
 4;114,116,125,123;,
 4;116,117,126,125;,
 4;117,118,127,126;,
 4;118,119,128,127;,
 4;119,120,129,128;,
 4;120,121,130,129;,
 4;121,122,131,130;,
 4;124,123,132,133;,
 4;123,125,134,132;,
 4;125,126,135,134;,
 4;126,127,136,135;,
 4;127,128,137,136;,
 4;128,129,138,137;,
 4;129,130,139,138;,
 4;130,131,140,139;,
 4;133,132,141,142;,
 4;132,134,143,141;,
 4;134,135,144,143;,
 4;135,136,145,144;,
 4;136,137,146,145;,
 4;137,138,147,146;,
 4;138,139,148,147;,
 4;139,140,149,148;,
 3;142,141,150;,
 3;141,143,151;,
 3;143,144,152;,
 3;144,145,153;,
 3;145,146,154;,
 3;146,147,155;,
 3;147,148,156;,
 3;148,149,157;;
 
 MeshMaterialList {
  2;
  128;
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
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.668000;0.540000;0.037600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.690400;0.690400;0.690400;1.000000;;
   5.000000;
   0.410000;0.410000;0.410000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  132;
  0.000000;1.000000;0.000000;,
  0.000000;0.957829;-0.287338;,
  0.203178;0.957829;-0.203178;,
  0.287338;0.957829;0.000000;,
  0.203178;0.957829;0.203178;,
  0.000000;0.957829;0.287338;,
  -0.203178;0.957829;0.203178;,
  -0.287338;0.957829;0.000000;,
  -0.203178;0.957829;-0.203178;,
  0.000000;0.809099;-0.587672;,
  0.415547;0.809099;-0.415547;,
  0.587672;0.809099;0.000000;,
  0.415547;0.809099;0.415547;,
  0.000000;0.809099;0.587672;,
  -0.415547;0.809099;0.415547;,
  -0.587672;0.809099;0.000000;,
  -0.415547;0.809099;-0.415547;,
  0.000000;0.576570;-0.817048;,
  0.577740;0.576570;-0.577740;,
  0.817048;0.576570;0.000000;,
  0.577740;0.576570;0.577740;,
  0.000000;0.576570;0.817048;,
  -0.577740;0.576570;0.577740;,
  -0.817048;0.576570;0.000000;,
  -0.577740;0.576570;-0.577740;,
  0.000000;0.455394;-0.890290;,
  0.629530;0.455394;-0.629530;,
  0.890290;0.455394;0.000000;,
  0.629530;0.455394;0.629530;,
  0.000000;0.455394;0.890290;,
  -0.629530;0.455394;0.629530;,
  -0.890290;0.455394;0.000000;,
  -0.629530;0.455394;-0.629530;,
  0.000000;-0.941154;-0.337978;,
  0.238987;-0.941154;-0.238987;,
  0.337978;-0.941154;-0.000000;,
  0.238987;-0.941154;0.238987;,
  0.000000;-0.941154;0.337978;,
  -0.238987;-0.941154;0.238987;,
  -0.337978;-0.941154;0.000000;,
  -0.238987;-0.941154;-0.238987;,
  0.000000;-0.987139;-0.159864;,
  0.113041;-0.987139;-0.113041;,
  0.159864;-0.987139;-0.000000;,
  0.113041;-0.987139;0.113041;,
  0.000000;-0.987139;0.159864;,
  -0.113041;-0.987139;0.113041;,
  -0.159864;-0.987139;0.000000;,
  -0.113041;-0.987139;-0.113041;,
  0.000000;-0.997890;-0.064921;,
  0.045906;-0.997890;-0.045906;,
  0.064921;-0.997890;-0.000000;,
  0.045906;-0.997890;0.045906;,
  0.000000;-0.997890;0.064921;,
  -0.045906;-0.997890;0.045906;,
  -0.064921;-0.997890;0.000000;,
  -0.045906;-0.997890;-0.045906;,
  0.000000;-1.000000;-0.000000;,
  0.000000;-0.891587;-0.452849;,
  0.320213;-0.891587;-0.320213;,
  0.452849;-0.891587;-0.000000;,
  0.320213;-0.891587;0.320213;,
  0.000000;-0.891587;0.452849;,
  -0.320213;-0.891587;0.320213;,
  -0.452849;-0.891587;0.000000;,
  -0.320213;-0.891587;-0.320213;,
  0.000000;1.000000;0.000000;,
  0.000000;0.999931;-0.011715;,
  0.008284;0.999931;-0.008284;,
  0.011715;0.999931;0.000000;,
  0.009791;0.999950;0.001860;,
  -0.006945;0.999887;-0.013324;,
  -0.054182;0.998462;-0.011740;,
  -0.086617;0.996242;0.000000;,
  -0.045706;0.998931;-0.006916;,
  0.002129;0.999065;-0.043183;,
  0.021110;0.999401;-0.027407;,
  0.023716;0.999690;-0.007631;,
  0.015192;0.999885;0.000148;,
  -0.028302;0.999575;-0.007051;,
  -0.065102;0.997283;-0.034477;,
  -0.087786;0.996113;-0.007220;,
  -0.061363;0.998110;-0.003447;,
  0.010975;0.994542;-0.103763;,
  0.057826;0.995975;-0.068484;,
  0.089242;0.995631;-0.027463;,
  0.065083;0.997528;0.026482;,
  -0.040472;0.996749;0.069664;,
  -0.085541;0.996318;0.005829;,
  -0.081873;0.996593;-0.010008;,
  -0.059481;0.997593;-0.035632;,
  0.017682;0.990118;-0.139116;,
  0.086151;0.991756;-0.094854;,
  0.148792;0.988077;-0.039566;,
  0.115606;0.991900;0.052632;,
  -0.038130;0.991878;0.121343;,
  -0.144922;0.988017;0.053110;,
  -0.155609;0.987803;-0.005553;,
  -0.083853;0.993698;-0.074385;,
  0.016309;-0.997809;-0.064126;,
  0.064773;-0.996748;-0.047942;,
  0.088823;-0.995675;-0.027233;,
  0.065083;-0.997528;0.026482;,
  -0.040992;-0.996730;0.069641;,
  -0.113184;-0.990008;0.084111;,
  -0.076208;-0.997089;-0.002231;,
  -0.027992;-0.995834;-0.086778;,
  0.002474;-0.999951;-0.009549;,
  0.020153;-0.999729;-0.011696;,
  0.023716;-0.999690;-0.007631;,
  0.015192;-0.999885;0.000148;,
  -0.028564;-0.999568;-0.006983;,
  -0.026757;-0.999320;0.025361;,
  0.047748;-0.998829;0.007765;,
  0.036374;-0.998883;-0.030162;,
  0.000000;-0.999931;-0.011715;,
  0.008284;-0.999931;-0.008284;,
  0.011715;-0.999931;-0.000000;,
  0.009791;-0.999950;0.001860;,
  -0.006945;-0.999887;-0.013324;,
  0.020973;-0.999739;-0.009070;,
  0.063743;-0.997966;-0.000000;,
  0.029421;-0.999521;-0.009579;,
  0.000000;-1.000000;-0.000000;,
  0.027618;-0.991514;-0.127036;,
  0.101855;-0.991148;-0.085153;,
  0.147963;-0.988219;-0.039108;,
  0.115606;-0.991900;0.052632;,
  -0.038643;-0.991880;0.121163;,
  -0.127382;-0.987216;0.095801;,
  -0.114118;-0.993267;-0.019952;,
  -0.065932;-0.989536;-0.128340;;
  128;
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;0,5,4;,
  3;0,6,5;,
  3;0,7,6;,
  3;0,8,7;,
  3;0,1,8;,
  4;1,2,10,9;,
  4;2,3,11,10;,
  4;3,4,12,11;,
  4;4,5,13,12;,
  4;5,6,14,13;,
  4;6,7,15,14;,
  4;7,8,16,15;,
  4;8,1,9,16;,
  4;9,10,18,17;,
  4;10,11,19,18;,
  4;11,12,20,19;,
  4;12,13,21,20;,
  4;13,14,22,21;,
  4;14,15,23,22;,
  4;15,16,24,23;,
  4;16,9,17,24;,
  4;17,18,26,25;,
  4;18,19,27,26;,
  4;19,20,28,27;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;23,24,32,31;,
  4;24,17,25,32;,
  4;58,59,34,33;,
  4;59,60,35,34;,
  4;60,61,36,35;,
  4;61,62,37,36;,
  4;62,63,38,37;,
  4;63,64,39,38;,
  4;64,65,40,39;,
  4;65,58,33,40;,
  4;33,34,42,41;,
  4;34,35,43,42;,
  4;35,36,44,43;,
  4;36,37,45,44;,
  4;37,38,46,45;,
  4;38,39,47,46;,
  4;39,40,48,47;,
  4;40,33,41,48;,
  4;41,42,50,49;,
  4;42,43,51,50;,
  4;43,44,52,51;,
  4;44,45,53,52;,
  4;45,46,54,53;,
  4;46,47,55,54;,
  4;47,48,56,55;,
  4;48,41,49,56;,
  3;49,50,57;,
  3;50,51,57;,
  3;51,52,57;,
  3;52,53,57;,
  3;53,54,57;,
  3;54,55,57;,
  3;55,56,57;,
  3;56,49,57;,
  3;66,68,67;,
  3;66,69,68;,
  3;66,70,69;,
  3;66,71,70;,
  3;66,72,71;,
  3;66,73,72;,
  3;66,74,73;,
  3;66,67,74;,
  4;67,68,76,75;,
  4;68,69,77,76;,
  4;69,70,78,77;,
  4;70,71,79,78;,
  4;71,72,80,79;,
  4;72,73,81,80;,
  4;73,74,82,81;,
  4;74,67,75,82;,
  4;75,76,84,83;,
  4;76,77,85,84;,
  4;77,78,86,85;,
  4;78,79,87,86;,
  4;79,80,88,87;,
  4;80,81,89,88;,
  4;81,82,90,89;,
  4;82,75,83,90;,
  4;83,84,92,91;,
  4;84,85,93,92;,
  4;85,86,94,93;,
  4;86,87,95,94;,
  4;87,88,96,95;,
  4;88,89,97,96;,
  4;89,90,98,97;,
  4;90,83,91,98;,
  4;124,125,100,99;,
  4;125,126,101,100;,
  4;126,127,102,101;,
  4;127,128,103,102;,
  4;128,129,104,103;,
  4;129,130,105,104;,
  4;130,131,106,105;,
  4;131,124,99,106;,
  4;99,100,108,107;,
  4;100,101,109,108;,
  4;101,102,110,109;,
  4;102,103,111,110;,
  4;103,104,112,111;,
  4;104,105,113,112;,
  4;105,106,114,113;,
  4;106,99,107,114;,
  4;107,108,116,115;,
  4;108,109,117,116;,
  4;109,110,118,117;,
  4;110,111,119,118;,
  4;111,112,120,119;,
  4;112,113,121,120;,
  4;113,114,122,121;,
  4;114,107,115,122;,
  3;115,116,123;,
  3;116,117,123;,
  3;117,118,123;,
  3;118,119,123;,
  3;119,120,123;,
  3;120,121,123;,
  3;121,122,123;,
  3;122,115,123;;
 }
 MeshTextureCoords {
  158;
  0.062500;0.000000;,
  0.125000;0.125000;,
  0.000000;0.125000;,
  0.187500;0.000000;,
  0.250000;0.125000;,
  0.312500;0.000000;,
  0.375000;0.125000;,
  0.437500;0.000000;,
  0.500000;0.125000;,
  0.562500;0.000000;,
  0.625000;0.125000;,
  0.687500;0.000000;,
  0.750000;0.125000;,
  0.812500;0.000000;,
  0.875000;0.125000;,
  0.937500;0.000000;,
  1.000000;0.125000;,
  0.125000;0.250000;,
  0.000000;0.250000;,
  0.250000;0.250000;,
  0.375000;0.250000;,
  0.500000;0.250000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  1.000000;0.250000;,
  0.125000;0.375000;,
  0.000000;0.375000;,
  0.250000;0.375000;,
  0.375000;0.375000;,
  0.500000;0.375000;,
  0.625000;0.375000;,
  0.750000;0.375000;,
  0.875000;0.375000;,
  1.000000;0.375000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.125000;0.625000;,
  0.000000;0.625000;,
  0.250000;0.625000;,
  0.375000;0.625000;,
  0.500000;0.625000;,
  0.625000;0.625000;,
  0.750000;0.625000;,
  0.875000;0.625000;,
  1.000000;0.625000;,
  0.125000;0.750000;,
  0.000000;0.750000;,
  0.250000;0.750000;,
  0.375000;0.750000;,
  0.500000;0.750000;,
  0.625000;0.750000;,
  0.750000;0.750000;,
  0.875000;0.750000;,
  1.000000;0.750000;,
  0.125000;0.875000;,
  0.000000;0.875000;,
  0.250000;0.875000;,
  0.375000;0.875000;,
  0.500000;0.875000;,
  0.625000;0.875000;,
  0.750000;0.875000;,
  0.875000;0.875000;,
  1.000000;0.875000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.062500;0.000000;,
  0.125000;0.125000;,
  0.000000;0.125000;,
  0.187500;0.000000;,
  0.250000;0.125000;,
  0.312500;0.000000;,
  0.375000;0.125000;,
  0.437500;0.000000;,
  0.500000;0.125000;,
  0.562500;0.000000;,
  0.625000;0.125000;,
  0.687500;0.000000;,
  0.750000;0.125000;,
  0.812500;0.000000;,
  0.875000;0.125000;,
  0.937500;0.000000;,
  1.000000;0.125000;,
  0.125000;0.250000;,
  0.000000;0.250000;,
  0.250000;0.250000;,
  0.375000;0.250000;,
  0.500000;0.250000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  1.000000;0.250000;,
  0.125000;0.375000;,
  0.000000;0.375000;,
  0.250000;0.375000;,
  0.375000;0.375000;,
  0.500000;0.375000;,
  0.625000;0.375000;,
  0.750000;0.375000;,
  0.875000;0.375000;,
  1.000000;0.375000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.125000;0.625000;,
  0.000000;0.625000;,
  0.250000;0.625000;,
  0.375000;0.625000;,
  0.500000;0.625000;,
  0.625000;0.625000;,
  0.750000;0.625000;,
  0.875000;0.625000;,
  1.000000;0.625000;,
  0.125000;0.750000;,
  0.000000;0.750000;,
  0.250000;0.750000;,
  0.375000;0.750000;,
  0.500000;0.750000;,
  0.625000;0.750000;,
  0.750000;0.750000;,
  0.875000;0.750000;,
  1.000000;0.750000;,
  0.125000;0.875000;,
  0.000000;0.875000;,
  0.250000;0.875000;,
  0.375000;0.875000;,
  0.500000;0.875000;,
  0.625000;0.875000;,
  0.750000;0.875000;,
  0.875000;0.875000;,
  1.000000;0.875000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;;
 }
}