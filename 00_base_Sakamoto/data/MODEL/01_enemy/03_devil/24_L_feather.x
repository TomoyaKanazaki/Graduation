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
 256;
 24.48896;72.69672;-0.94703;,
 -2.56258;-4.98500;2.31706;,
 -1.22496;-7.14329;1.67725;,
 25.82671;71.39961;-1.58681;,
 -0.67077;-7.59689;0.13254;,
 26.38078;70.86196;-3.13149;,
 -1.22494;-7.14329;-1.41213;,
 25.82672;71.39961;-4.67618;,
 -2.56258;-4.98500;-2.05192;,
 24.48896;72.69672;-5.31601;,
 -3.90035;-4.95324;-1.41213;,
 23.15126;73.99413;-4.67618;,
 -4.45443;-4.49972;0.13254;,
 22.59713;74.53161;-3.13149;,
 -3.90035;-4.95324;1.67725;,
 23.15126;73.99413;-1.58681;,
 -2.56258;-4.98500;2.31706;,
 24.48896;72.69672;-0.94703;,
 24.48896;72.69672;-3.13149;,
 24.48896;72.69672;-3.13149;,
 24.48896;72.69672;-3.13149;,
 24.48896;72.69672;-3.13149;,
 24.48896;72.69672;-3.13149;,
 24.48896;72.69672;-3.13149;,
 24.48896;72.69672;-3.13149;,
 24.48896;72.69672;-3.13149;,
 -2.56262;-4.98500;0.13254;,
 -2.56262;-4.98500;0.13254;,
 -2.56262;-4.98500;0.13254;,
 -2.56262;-4.98500;0.13254;,
 -2.56262;-4.98500;0.13254;,
 -2.56262;-4.98500;0.13254;,
 -2.56262;-4.98500;0.13254;,
 -2.56262;-4.98500;0.13254;,
 24.83434;-31.56469;-13.45929;,
 26.37900;-23.89553;-10.49598;,
 24.83434;-24.20078;-9.88250;,
 24.83434;-31.56469;-13.45929;,
 27.01883;-23.15859;-11.97704;,
 24.83434;-31.56469;-13.45929;,
 26.37900;-22.42158;-13.45809;,
 24.83434;-31.56469;-13.45929;,
 24.83434;-22.11635;-14.07156;,
 24.83434;-31.56469;-13.45929;,
 23.28968;-22.42158;-13.45809;,
 24.83434;-31.56469;-13.45929;,
 22.64985;-23.15859;-11.97704;,
 24.83434;-31.56469;-13.45929;,
 23.28968;-23.89553;-10.49596;,
 24.83434;-31.56469;-13.45929;,
 24.83434;-24.20078;-9.88250;,
 24.83434;78.34341;-2.24576;,
 24.83431;75.39660;-0.94703;,
 26.37900;75.39660;-1.58681;,
 25.46062;78.34341;-2.50520;,
 27.01883;75.39660;-3.13149;,
 25.72008;78.34341;-3.13149;,
 26.37900;75.39660;-4.67618;,
 25.46062;78.34341;-3.75781;,
 24.83431;75.39660;-5.31598;,
 24.83431;78.34341;-4.01722;,
 23.28968;75.39660;-4.67618;,
 24.20808;78.34341;-3.75781;,
 22.64991;75.39660;-3.13149;,
 23.94865;78.34341;-3.13149;,
 23.28968;75.39660;-1.58681;,
 24.20808;78.34341;-2.50520;,
 24.83431;75.39660;-0.94703;,
 24.83434;78.34341;-2.24576;,
 22.71686;86.74070;-3.03950;,
 22.78191;86.74070;-3.06643;,
 22.71686;86.74070;-3.13149;,
 22.80885;86.74070;-3.13149;,
 22.71686;86.74070;-3.13149;,
 22.78191;86.74070;-3.19658;,
 22.71686;86.74070;-3.13149;,
 22.71686;86.74070;-3.22353;,
 22.71686;86.74070;-3.13149;,
 22.65181;86.74070;-3.19658;,
 22.71686;86.74070;-3.13149;,
 22.62482;86.74070;-3.13149;,
 22.71686;86.74070;-3.13149;,
 22.65181;86.74070;-3.06643;,
 22.71686;86.74070;-3.13149;,
 22.71686;86.74070;-3.03950;,
 22.71686;86.74070;-3.13149;,
 26.37900;20.07746;-1.58681;,
 27.01883;20.07746;-3.13149;,
 24.83431;20.07746;-0.94703;,
 26.37900;20.07746;-4.67618;,
 24.83431;20.07746;-0.94703;,
 23.28968;20.07746;-1.58681;,
 24.83431;20.07746;-5.31598;,
 22.64991;20.07746;-3.13149;,
 23.28968;20.07746;-4.67618;,
 24.90859;72.48396;-3.31388;,
 24.90859;71.75242;-1.17325;,
 26.24628;73.23815;-1.64624;,
 24.90859;72.48396;-3.31388;,
 26.80037;74.28204;-3.09613;,
 24.90859;72.48396;-3.31388;,
 26.24628;74.27273;-4.67354;,
 24.90859;72.48396;-3.31388;,
 24.90859;73.21578;-5.45449;,
 24.90859;72.48396;-3.31388;,
 23.57084;71.73004;-4.98150;,
 24.90859;72.48396;-3.31388;,
 23.01674;70.68617;-3.53164;,
 24.90859;72.48396;-3.31388;,
 23.57084;70.69537;-1.95420;,
 24.90859;72.48396;-3.31388;,
 24.90859;71.75242;-1.17325;,
 54.98662;-13.64771;-22.19710;,
 53.18919;-5.49769;-16.60698;,
 51.85141;-7.18066;-16.56080;,
 54.98662;-13.64771;-22.19710;,
 53.74326;-3.50692;-17.64805;,
 54.98662;-13.64771;-22.19710;,
 53.18919;-2.37446;-19.07423;,
 54.98662;-13.64771;-22.19710;,
 51.85141;-2.76381;-20.05000;,
 54.98662;-13.64771;-22.19710;,
 50.51375;-4.44686;-20.00381;,
 54.98662;-13.64771;-22.19710;,
 49.95962;-6.43763;-18.96272;,
 54.98662;-13.64771;-22.19710;,
 50.51375;-7.57003;-17.53656;,
 54.98662;-13.64771;-22.19710;,
 51.85141;-7.18066;-16.56080;,
 43.30219;24.47800;-5.92003;,
 41.96443;22.99241;-5.44704;,
 43.85626;25.52199;-7.36992;,
 41.96443;22.99241;-5.44704;,
 40.62675;21.93531;-6.22797;,
 43.30219;25.51274;-8.94736;,
 40.07266;21.92594;-7.80543;,
 41.96443;24.45557;-9.72831;,
 40.62675;22.96993;-9.25530;,
 24.52173;72.15820;-0.94703;,
 49.57648;58.29762;-14.90268;,
 50.34892;60.54491;-15.54248;,
 25.29406;74.40527;-1.58681;,
 50.66872;61.47550;-17.08714;,
 25.61394;75.33616;-3.13149;,
 50.34892;60.54491;-18.63183;,
 25.29406;74.40527;-4.67618;,
 49.57648;58.29762;-19.27166;,
 24.52173;72.15820;-5.31598;,
 48.80423;56.05062;-18.63183;,
 23.74939;69.91104;-4.67618;,
 48.48431;55.11980;-17.08714;,
 23.42947;68.98014;-3.13149;,
 48.80416;56.05062;-15.54246;,
 23.74939;69.91104;-1.58681;,
 49.57648;58.29762;-14.90268;,
 24.52173;72.15820;-0.94703;,
 24.52173;72.15820;-3.13149;,
 24.52173;72.15820;-3.13149;,
 24.52173;72.15820;-3.13149;,
 24.52173;72.15820;-3.13149;,
 24.52173;72.15820;-3.13149;,
 24.52173;72.15820;-3.13149;,
 24.52173;72.15820;-3.13149;,
 24.52173;72.15820;-3.13149;,
 49.57648;58.29762;-17.08714;,
 49.57648;58.29762;-17.08714;,
 49.57648;58.29762;-17.08714;,
 49.57648;58.29762;-17.08714;,
 49.57648;58.29762;-17.08714;,
 49.57648;58.29762;-17.08714;,
 49.57648;58.29762;-17.08714;,
 49.57648;58.29762;-17.08714;,
 48.64367;60.25260;-16.75251;,
 48.64367;59.55241;-14.60816;,
 50.13572;60.41666;-15.15992;,
 48.64367;60.25260;-16.75251;,
 50.75375;61.18486;-16.64462;,
 48.64367;60.25260;-16.75251;,
 50.13572;61.40701;-18.19251;,
 48.64367;60.25260;-16.75251;,
 48.64367;60.95282;-18.89687;,
 48.64367;60.25260;-16.75251;,
 47.15165;60.08849;-18.34506;,
 48.64367;60.25260;-16.75251;,
 46.53368;59.32018;-16.86036;,
 48.64367;60.25260;-16.75251;,
 47.15163;59.09811;-15.31249;,
 48.64367;60.25260;-16.75251;,
 48.64367;59.55241;-14.60816;,
 74.59601;-3.56894;-38.38939;,
 72.34214;1.01994;-30.99154;,
 70.85004;-0.15929;-30.71500;,
 74.59601;-3.56894;-38.38939;,
 72.96004;2.78698;-32.13470;,
 74.59601;-3.56894;-38.38939;,
 72.34214;4.10694;-33.47484;,
 74.59601;-3.56894;-38.38939;,
 70.85004;4.20631;-34.22704;,
 74.59601;-3.56894;-38.38939;,
 69.35803;3.02706;-33.95057;,
 74.59601;-3.56894;-38.38939;,
 68.73998;1.26005;-32.80734;,
 74.59601;-3.56894;-38.38939;,
 69.35803;-0.05986;-31.46712;,
 74.59601;-3.56894;-38.38939;,
 70.85004;-0.15929;-30.71500;,
 61.91566;30.75734;-20.33789;,
 61.29769;30.97936;-21.88581;,
 61.29769;29.98908;-18.85322;,
 59.80553;30.52510;-22.59015;,
 59.80553;29.12481;-18.30143;,
 58.31364;29.66086;-22.03835;,
 59.80553;29.12481;-18.30143;,
 58.31364;28.67066;-19.00579;,
 57.69540;28.89266;-20.55369;,
 53.31947;27.38969;-16.78239;,
 60.30887;8.45548;-23.75032;,
 69.40217;0.15115;-32.54624;,
 59.52895;29.57392;-20.99003;,
 48.10677;60.17719;-17.27904;,
 40.90586;63.17409;-12.89419;,
 53.31947;27.38969;-16.78239;,
 45.78203;24.73826;-11.67493;,
 56.18743;6.59687;-19.94464;,
 41.09449;23.08932;-8.49867;,
 52.39542;-9.11642;-19.69092;,
 32.16503;66.81199;-7.57169;,
 26.72922;69.07440;-4.26165;,
 45.78203;24.73826;-11.67493;,
 26.91321;64.22614;-2.83812;,
 27.10866;64.22614;-2.83812;,
 40.47957;22.56857;-7.26630;,
 33.60018;22.56857;-4.67178;,
 33.60018;22.56857;-4.67178;,
 51.62960;-5.42798;-16.84395;,
 42.00091;-0.84131;-4.67178;,
 31.42154;21.99359;-4.67178;,
 34.69308;-3.43365;-6.41009;,
 26.10697;-21.85489;-11.03062;,
 25.63069;20.46517;-4.67178;,
 26.85981;64.22614;-2.83812;,
 26.71783;64.22614;-2.83812;,
 31.42154;21.99359;-4.67178;,
 18.04106;20.83432;-3.34067;,
 24.02375;20.83432;-3.34067;,
 23.18118;65.70116;-3.34067;,
 22.93039;65.70116;-3.34067;,
 22.67961;65.70116;-3.34067;,
 -3.67199;-4.85649;0.92288;,
 18.04106;5.95293;-4.67684;,
 3.16633;-9.36316;-2.79252;,
 10.50594;-14.44874;-3.34067;,
 18.04106;-3.62310;-5.53657;,
 24.05035;-6.49370;-9.18129;,
 24.06752;-24.07854;-12.93958;,
 18.04106;5.95293;-4.67684;;
 
 196;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 4;9,8,10,11;,
 4;11,10,12,13;,
 4;13,12,14,15;,
 4;15,14,16,17;,
 3;18,0,3;,
 3;19,3,5;,
 3;20,5,7;,
 3;21,7,9;,
 3;22,9,11;,
 3;23,11,13;,
 3;24,13,15;,
 3;25,15,17;,
 3;26,2,1;,
 3;27,4,2;,
 3;28,6,4;,
 3;29,8,6;,
 3;30,10,8;,
 3;31,12,10;,
 3;32,14,12;,
 3;33,16,14;,
 3;34,35,36;,
 3;37,38,35;,
 3;39,40,38;,
 3;41,42,40;,
 3;43,44,42;,
 3;45,46,44;,
 3;47,48,46;,
 3;49,50,48;,
 4;51,52,53,54;,
 4;54,53,55,56;,
 4;56,55,57,58;,
 4;58,57,59,60;,
 4;60,59,61,62;,
 4;62,61,63,64;,
 4;64,63,65,66;,
 4;66,65,67,68;,
 4;69,51,54,70;,
 3;71,69,70;,
 4;70,54,56,72;,
 3;73,70,72;,
 4;72,56,58,74;,
 3;75,72,74;,
 4;74,58,60,76;,
 3;77,74,76;,
 4;76,60,62,78;,
 3;79,76,78;,
 4;78,62,64,80;,
 3;81,78,80;,
 4;80,64,66,82;,
 3;83,80,82;,
 4;82,66,68,84;,
 3;85,82,84;,
 4;86,87,55,53;,
 4;86,35,38,87;,
 4;86,88,36,35;,
 4;86,53,52,88;,
 4;87,89,57,55;,
 4;87,38,40,89;,
 4;90,91,48,50;,
 4;90,67,65,91;,
 4;89,92,59,57;,
 4;89,40,42,92;,
 4;91,93,46,48;,
 4;91,65,63,93;,
 4;92,94,61,59;,
 4;92,42,44,94;,
 4;93,94,44,46;,
 4;93,63,61,94;,
 3;95,96,97;,
 3;98,97,99;,
 3;100,99,101;,
 3;102,101,103;,
 3;104,103,105;,
 3;106,105,107;,
 3;108,107,109;,
 3;110,109,111;,
 3;112,113,114;,
 3;115,116,113;,
 3;117,118,116;,
 3;119,120,118;,
 3;121,122,120;,
 3;123,124,122;,
 3;125,126,124;,
 3;127,128,126;,
 4;129,130,114,113;,
 4;129,97,96,130;,
 4;129,131,99,97;,
 4;129,113,116,131;,
 4;132,133,126,128;,
 4;132,111,109,133;,
 4;131,134,101,99;,
 4;131,116,118,134;,
 4;133,135,124,126;,
 4;133,109,107,135;,
 4;134,136,103,101;,
 4;134,118,120,136;,
 4;135,137,122,124;,
 4;135,107,105,137;,
 4;136,137,105,103;,
 4;136,120,122,137;,
 4;138,139,140,141;,
 4;141,140,142,143;,
 4;143,142,144,145;,
 4;145,144,146,147;,
 4;147,146,148,149;,
 4;149,148,150,151;,
 4;151,150,152,153;,
 4;153,152,154,155;,
 3;156,138,141;,
 3;157,141,143;,
 3;158,143,145;,
 3;159,145,147;,
 3;160,147,149;,
 3;161,149,151;,
 3;162,151,153;,
 3;163,153,155;,
 3;164,140,139;,
 3;165,142,140;,
 3;166,144,142;,
 3;167,146,144;,
 3;168,148,146;,
 3;169,150,148;,
 3;170,152,150;,
 3;171,154,152;,
 3;172,173,174;,
 3;175,174,176;,
 3;177,176,178;,
 3;179,178,180;,
 3;181,180,182;,
 3;183,182,184;,
 3;185,184,186;,
 3;187,186,188;,
 3;189,190,191;,
 3;192,193,190;,
 3;194,195,193;,
 3;196,197,195;,
 3;198,199,197;,
 3;200,201,199;,
 3;202,203,201;,
 3;204,205,203;,
 4;206,207,178,176;,
 4;206,193,195,207;,
 4;206,208,190,193;,
 4;206,176,174,208;,
 4;207,209,180,178;,
 4;207,195,197,209;,
 4;208,210,191,190;,
 4;208,174,173,210;,
 4;209,211,182,180;,
 4;209,197,199,211;,
 4;212,213,203,205;,
 4;212,188,186,213;,
 4;211,214,184,182;,
 4;211,199,201,214;,
 4;213,214,201,203;,
 4;213,186,184,214;,
 4;215,216,217,218;,
 4;215,218,219,220;,
 4;216,221,218,217;,
 4;220,219,218,221;,
 4;222,223,216,215;,
 4;222,224,225,223;,
 4;222,226,227,224;,
 4;222,215,220,226;,
 4;223,228,221,216;,
 4;223,225,224,228;,
 4;226,228,224,227;,
 4;226,220,221,228;,
 4;229,230,231,232;,
 4;229,233,231,230;,
 4;232,231,234,235;,
 4;233,235,234,231;,
 4;236,237,238,239;,
 4;236,232,235,237;,
 4;236,240,229,232;,
 4;236,239,241,240;,
 4;237,242,239,238;,
 4;237,235,233,242;,
 4;240,242,233,229;,
 4;240,241,239,242;,
 4;243,244,245,246;,
 4;243,246,247,248;,
 4;244,243,246,245;,
 4;246,243,248,247;,
 4;249,250,251,252;,
 4;249,243,248,250;,
 4;249,253,244,243;,
 4;249,252,254,253;,
 4;250,255,252,251;,
 4;250,248,243,255;,
 4;253,255,243,244;,
 4;253,254,252,255;;
 
 MeshMaterialList {
  12;
  196;
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  10,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8,
  8;;
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
  243;
  0.696187;0.717860;0.000001;,
  -0.011015;0.045394;0.998908;,
  0.659173;-0.197213;0.725671;,
  0.945400;-0.325901;-0.002717;,
  0.622725;-0.245879;-0.742804;,
  -0.060837;-0.020741;-0.997932;,
  -0.678401;0.203668;-0.705897;,
  -0.946020;0.324097;0.002714;,
  -0.656498;0.255006;0.709917;,
  -0.531889;-0.846814;-0.000001;,
  -0.000006;0.207316;0.978274;,
  0.691736;0.207314;0.691754;,
  0.978274;0.207317;0.000003;,
  0.691736;0.207317;-0.691753;,
  -0.000010;0.207316;-0.978274;,
  -0.691754;0.207317;-0.691734;,
  -0.978274;0.207315;0.000003;,
  -0.691751;0.207320;0.691736;,
  0.000002;-0.319524;0.947578;,
  0.720626;-0.257681;0.643660;,
  0.991632;-0.117571;-0.053329;,
  0.691244;0.014931;-0.722467;,
  -0.000002;0.068899;-0.997624;,
  -0.691248;0.014931;-0.722464;,
  -0.991632;-0.117570;-0.053330;,
  -0.720628;-0.257681;0.643658;,
  0.720749;-0.371340;0.585344;,
  -0.001728;0.252772;0.967524;,
  0.664223;0.333972;0.668783;,
  0.930689;0.365811;0.000029;,
  0.664254;0.333986;-0.668745;,
  -0.001737;0.252774;-0.967524;,
  -0.696389;0.166925;-0.697982;,
  -0.991443;0.130538;0.000030;,
  -0.696345;0.166945;0.698021;,
  0.000000;1.000000;0.000000;,
  -0.003434;0.093240;0.995638;,
  0.677806;0.262130;0.686926;,
  0.944832;0.327555;0.000052;,
  0.677875;0.262151;-0.686850;,
  -0.003443;0.093237;-0.995638;,
  -0.703094;-0.083115;-0.706223;,
  -0.987802;-0.155714;0.000054;,
  -0.703016;-0.083081;0.706305;,
  0.709705;-0.070219;0.700990;,
  1.000000;0.000194;0.000045;,
  -0.000004;-0.099541;0.995034;,
  0.709791;0.071566;-0.700768;,
  -0.709716;-0.070218;0.700980;,
  -0.000006;0.101863;-0.994798;,
  -1.000000;0.000195;0.000045;,
  -0.709802;0.071565;-0.700756;,
  -0.683567;0.690661;0.236059;,
  -0.683580;0.690659;0.236027;,
  -0.683607;0.690637;0.236012;,
  -0.683601;0.690638;0.236026;,
  -0.683576;0.690644;0.236082;,
  -0.683559;0.690649;0.236117;,
  -0.683547;0.690671;0.236089;,
  -0.683529;0.690696;0.236066;,
  -0.683539;0.690691;0.236052;,
  0.091266;-0.477851;0.873687;,
  0.722392;-0.144682;0.676178;,
  0.976327;0.172545;0.130438;,
  0.739219;0.365448;-0.565688;,
  0.059433;0.289232;-0.955412;,
  -0.575824;0.014219;-0.817450;,
  -0.895741;-0.326686;-0.301536;,
  -0.668830;-0.586983;0.456198;,
  0.929012;-0.095323;0.357562;,
  0.643038;0.049980;0.764202;,
  0.004823;-0.217300;0.976093;,
  0.946627;0.293079;0.134170;,
  -0.685214;-0.374412;0.624737;,
  0.684960;0.375381;-0.624435;,
  -0.946761;-0.292960;-0.133487;,
  -0.006773;0.219694;-0.975545;,
  -0.644739;-0.048494;-0.762863;,
  -0.945703;0.325032;0.000000;,
  0.417486;-0.148036;0.896544;,
  0.598003;0.359926;0.716132;,
  0.615276;0.612393;0.496397;,
  -0.073129;0.638885;-0.765819;,
  -0.417492;0.148024;-0.896543;,
  -0.597999;-0.359913;-0.716142;,
  -0.615277;-0.612353;-0.496446;,
  0.073154;-0.638861;0.765836;,
  0.945701;-0.325038;0.000006;,
  -0.399588;0.871409;0.284562;,
  -0.399592;0.871414;0.284543;,
  -0.399551;0.871428;0.284556;,
  -0.399556;0.871419;0.284576;,
  -0.399573;0.871413;0.284570;,
  -0.399583;0.871415;0.284552;,
  -0.399597;0.871401;0.284575;,
  -0.399620;0.871389;0.284577;,
  -0.399631;0.871395;0.284546;,
  0.123645;-0.652300;0.747808;,
  0.750864;-0.219641;0.622865;,
  0.964909;0.208824;0.159195;,
  0.708309;0.506358;-0.491834;,
  -0.015173;0.432433;-0.901539;,
  -0.631878;0.061294;-0.772640;,
  -0.872914;-0.353355;-0.336394;,
  -0.639108;-0.709041;0.297997;,
  0.941183;-0.140870;0.307131;,
  0.938992;0.313020;0.142522;,
  0.681069;0.419722;-0.599982;,
  0.653714;0.042088;0.755571;,
  -0.014901;0.258283;-0.965954;,
  0.013062;-0.253351;0.967286;,
  -0.653958;-0.041807;-0.755375;,
  -0.682290;-0.414226;0.602410;,
  -0.939319;-0.312361;-0.141812;,
  0.540976;0.098659;0.835231;,
  0.541146;0.092798;0.835793;,
  0.619256;-0.079235;0.781181;,
  0.546210;-0.078894;0.833925;,
  0.583090;0.009761;0.812349;,
  0.545694;0.006978;0.837955;,
  0.584959;0.020857;0.810794;,
  0.624090;-0.055380;0.779387;,
  0.540969;0.096898;0.835442;,
  -0.584959;-0.020857;-0.810794;,
  0.566448;0.019464;0.823868;,
  0.588283;-0.055278;0.806763;,
  0.541054;0.093968;0.835722;,
  -0.566448;-0.019464;-0.823868;,
  -0.599288;-0.126554;-0.790467;,
  0.093909;0.107601;-0.989749;,
  -0.179406;0.015341;-0.983656;,
  -0.005566;0.042020;-0.999101;,
  -0.401248;-0.057306;-0.914175;,
  0.044251;0.074945;-0.996205;,
  -0.092850;0.029176;-0.995253;,
  -0.178648;-0.005740;-0.983896;,
  0.178648;0.005740;0.983896;,
  -0.269081;-0.043191;-0.962149;,
  0.045839;0.059817;-0.997156;,
  0.097134;0.077064;-0.992283;,
  -0.005566;0.042402;-0.999085;,
  -0.045839;-0.059817;0.997156;,
  0.466877;-0.132725;0.874306;,
  0.179002;0.006457;0.983828;,
  0.000000;0.000000;1.000000;,
  0.109808;-0.011352;0.993888;,
  0.327652;-0.064055;0.942625;,
  0.054988;-0.005685;0.998471;,
  0.144948;-0.079121;0.986271;,
  0.073305;-0.073634;0.994587;,
  0.216053;-0.084210;0.972743;,
  0.280746;-0.108315;0.953651;,
  0.250320;-0.075040;0.965251;,
  0.310812;-0.141633;0.939859;,
  -0.280746;0.108315;-0.953651;,
  0.696166;0.717881;0.000000;,
  0.696157;0.717889;-0.000067;,
  0.696197;0.717851;0.000004;,
  0.696155;0.717892;0.000071;,
  0.696164;0.717883;0.000000;,
  0.696218;0.717830;-0.000005;,
  0.696222;0.717827;0.000000;,
  0.696218;0.717830;0.000005;,
  -0.829099;-0.555993;-0.058872;,
  -0.496752;-0.867893;0.000011;,
  -0.809870;-0.586609;-0.000005;,
  -0.829097;-0.555997;0.058867;,
  -0.496748;-0.867895;-0.000011;,
  -0.135287;-0.986057;0.096892;,
  -0.248471;-0.968639;0.000002;,
  -0.135287;-0.986058;-0.096890;,
  0.661879;-0.111722;-0.741239;,
  -0.661878;-0.111721;-0.741239;,
  -0.720746;-0.371342;0.585346;,
  -0.156385;0.105485;-0.982047;,
  -0.632502;-0.707403;0.315471;,
  0.612490;0.146154;0.776850;,
  -0.030490;-0.097853;0.994734;,
  0.941887;0.320699;0.100008;,
  -0.683780;-0.297577;0.666253;,
  0.683783;0.297579;-0.666250;,
  -0.941886;-0.320698;-0.100013;,
  0.030487;0.097852;-0.994734;,
  -0.612490;-0.146153;-0.776850;,
  0.161934;0.830047;-0.533666;,
  -0.161888;-0.830023;0.533717;,
  -0.945701;0.325038;0.000000;,
  -0.945702;0.325035;0.000016;,
  -0.945705;0.325026;0.000000;,
  -0.945702;0.325035;-0.000016;,
  -0.945704;0.325029;0.000003;,
  -0.945705;0.325027;-0.000000;,
  -0.945704;0.325029;-0.000003;,
  0.945698;-0.325046;-0.000012;,
  0.945697;-0.325051;-0.000000;,
  0.945701;-0.325039;-0.000000;,
  0.945698;-0.325046;0.000012;,
  0.945701;-0.325037;-0.000000;,
  0.945706;-0.325022;0.000003;,
  0.945706;-0.325024;0.000022;,
  0.945702;-0.325036;0.000019;,
  0.006589;0.474416;-0.880276;,
  -0.783375;-0.402758;-0.473402;,
  -0.216205;-0.927043;0.306343;,
  0.676606;0.328211;-0.659153;,
  0.938131;0.332332;0.097290;,
  0.633638;0.140091;0.760840;,
  0.006806;0.122952;-0.992389;,
  -0.006838;-0.122964;0.992388;,
  -0.633634;-0.140088;-0.760844;,
  -0.676593;-0.328208;0.659167;,
  -0.938132;-0.332331;-0.097283;,
  -0.624090;0.055380;-0.779387;,
  -0.583090;-0.009761;-0.812349;,
  -0.619256;0.079235;-0.781181;,
  -0.540969;-0.096898;-0.835442;,
  -0.540976;-0.098659;-0.835231;,
  -0.588283;0.055278;-0.806763;,
  -0.546210;0.078894;-0.833925;,
  -0.545694;-0.006978;-0.837955;,
  -0.541054;-0.093968;-0.835722;,
  -0.541146;-0.092798;-0.835793;,
  0.092850;-0.029176;0.995253;,
  0.401248;0.057306;0.914175;,
  0.179406;-0.015341;0.983656;,
  0.269081;0.043191;0.962149;,
  0.599288;0.126554;0.790467;,
  -0.097134;-0.077064;0.992283;,
  -0.044251;-0.074945;0.996205;,
  -0.093909;-0.107601;0.989749;,
  0.005566;-0.042402;0.999085;,
  0.005566;-0.042020;0.999101;,
  -0.073305;0.073634;-0.994587;,
  -0.144948;0.079121;-0.986271;,
  -0.054988;0.005685;-0.998471;,
  0.000000;0.000000;-1.000000;,
  -0.216053;0.084210;-0.972743;,
  -0.109808;0.011352;-0.993888;,
  -0.250320;0.075040;-0.965251;,
  -0.327652;0.064055;-0.942625;,
  -0.179002;-0.006457;-0.983828;,
  -0.310812;0.141633;-0.939859;,
  -0.466877;0.132725;-0.874306;;
  196;
  4;1,1,2,2;,
  4;2,2,3,3;,
  4;3,3,4,4;,
  4;4,4,5,5;,
  4;5,5,6,6;,
  4;6,6,7,7;,
  4;7,7,8,8;,
  4;8,8,1,1;,
  3;0,155,156;,
  3;0,156,157;,
  3;0,157,158;,
  3;0,158,159;,
  3;0,159,160;,
  3;0,160,161;,
  3;0,161,162;,
  3;0,162,155;,
  3;9,163,164;,
  3;9,165,163;,
  3;9,166,165;,
  3;9,167,166;,
  3;9,168,167;,
  3;9,169,168;,
  3;9,170,169;,
  3;9,164,170;,
  3;26,19,18;,
  3;26,20,19;,
  3;171,21,20;,
  3;171,22,21;,
  3;172,23,22;,
  3;172,24,23;,
  3;173,25,24;,
  3;173,18,25;,
  4;27,10,11,28;,
  4;28,11,12,29;,
  4;29,12,13,30;,
  4;30,13,14,31;,
  4;31,14,15,32;,
  4;32,15,16,33;,
  4;33,16,17,34;,
  4;34,17,10,27;,
  4;36,27,28,37;,
  3;35,35,35;,
  4;37,28,29,38;,
  3;35,35,35;,
  4;38,29,30,39;,
  3;35,35,35;,
  4;39,30,31,40;,
  3;35,35,35;,
  4;40,31,32,41;,
  3;35,35,35;,
  4;41,32,33,42;,
  3;35,35,35;,
  4;42,33,34,43;,
  3;35,35,35;,
  4;43,34,27,36;,
  3;35,35,35;,
  4;44,45,12,11;,
  4;44,19,20,45;,
  4;44,46,18,19;,
  4;44,11,10,46;,
  4;45,47,13,12;,
  4;45,20,21,47;,
  4;46,48,25,18;,
  4;46,10,17,48;,
  4;47,49,14,13;,
  4;47,21,22,49;,
  4;48,50,24,25;,
  4;48,17,16,50;,
  4;49,51,15,14;,
  4;49,22,23,51;,
  4;50,51,23,24;,
  4;50,16,15,51;,
  3;52,53,54;,
  3;52,54,55;,
  3;52,55,56;,
  3;52,56,57;,
  3;52,57,58;,
  3;52,58,59;,
  3;52,59,60;,
  3;52,60,53;,
  3;69,62,61;,
  3;69,63,62;,
  3;69,64,63;,
  3;174,65,64;,
  3;174,66,65;,
  3;174,67,66;,
  3;175,68,67;,
  3;175,61,68;,
  4;70,71,61,62;,
  4;70,176,177,71;,
  4;70,72,178,176;,
  4;70,62,63,72;,
  4;71,73,68,61;,
  4;71,177,179,73;,
  4;72,74,180,178;,
  4;72,63,64,74;,
  4;73,75,67,68;,
  4;73,179,181,75;,
  4;74,76,182,180;,
  4;74,64,65,76;,
  4;75,77,66,67;,
  4;75,181,183,77;,
  4;76,77,183,182;,
  4;76,65,66,77;,
  4;79,79,80,80;,
  4;80,80,81,81;,
  4;184,184,82,82;,
  4;82,82,83,83;,
  4;83,83,84,84;,
  4;84,84,85,85;,
  4;185,185,86,86;,
  4;86,86,79,79;,
  3;78,186,187;,
  3;78,187,188;,
  3;78,188,189;,
  3;78,189,186;,
  3;78,186,190;,
  3;78,190,191;,
  3;78,191,192;,
  3;78,192,186;,
  3;87,193,194;,
  3;87,195,193;,
  3;87,196,195;,
  3;87,197,196;,
  3;87,198,197;,
  3;87,199,198;,
  3;87,200,199;,
  3;87,194,200;,
  3;88,89,90;,
  3;88,90,91;,
  3;88,91,92;,
  3;88,92,93;,
  3;88,93,94;,
  3;88,94,95;,
  3;88,95,96;,
  3;88,96,89;,
  3;105,98,97;,
  3;105,99,98;,
  3;105,100,99;,
  3;201,101,100;,
  3;201,102,101;,
  3;202,103,102;,
  3;202,104,103;,
  3;203,97,104;,
  4;106,107,204,205;,
  4;106,99,100,107;,
  4;106,108,98,99;,
  4;106,205,206,108;,
  4;107,109,207,204;,
  4;107,100,101,109;,
  4;108,110,97,98;,
  4;108,206,208,110;,
  4;109,111,209,207;,
  4;109,101,102,111;,
  4;110,112,104,97;,
  4;110,208,210,112;,
  4;111,113,211,209;,
  4;111,102,103,113;,
  4;112,113,103,104;,
  4;112,210,211,113;,
  4;120,121,116,118;,
  4;120,118,114,122;,
  4;212,123,213,214;,
  4;215,216,213,123;,
  4;124,125,121,120;,
  4;124,119,117,125;,
  4;124,126,115,119;,
  4;124,120,122,126;,
  4;217,127,123,212;,
  4;217,218,219,127;,
  4;220,127,219,221;,
  4;220,215,123,127;,
  4;134,130,132,135;,
  4;222,136,223,224;,
  4;135,132,128,137;,
  4;136,225,226,223;,
  4;138,139,129,133;,
  4;138,135,137,139;,
  4;138,140,134,135;,
  4;138,133,131,140;,
  4;227,141,228,229;,
  4;227,225,136,141;,
  4;230,141,136,222;,
  4;230,231,228,141;,
  4;148,149,144,147;,
  4;148,147,145,150;,
  4;232,233,234,235;,
  4;234,233,236,237;,
  4;151,152,143,146;,
  4;151,148,150,152;,
  4;151,153,149,148;,
  4;151,146,142,153;,
  4;238,154,239,240;,
  4;238,236,233,154;,
  4;241,154,233,232;,
  4;241,242,239,154;;
 }
 MeshTextureCoords {
  256;
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.125000;1.000000;,
  0.125000;0.000000;,
  0.250000;1.000000;,
  0.250000;0.000000;,
  0.375000;1.000000;,
  0.375000;0.000000;,
  0.500000;1.000000;,
  0.500000;0.000000;,
  0.625000;1.000000;,
  0.625000;0.000000;,
  0.750000;1.000000;,
  0.750000;0.000000;,
  0.875000;1.000000;,
  0.875000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
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
  0.062500;1.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.187500;1.000000;,
  0.250000;1.000000;,
  0.312500;1.000000;,
  0.375000;1.000000;,
  0.437500;1.000000;,
  0.500000;1.000000;,
  0.562500;1.000000;,
  0.625000;1.000000;,
  0.687500;1.000000;,
  0.750000;1.000000;,
  0.812500;1.000000;,
  0.875000;1.000000;,
  0.937500;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;0.000000;,
  0.250000;0.000000;,
  0.250000;0.000000;,
  0.375000;0.000000;,
  0.375000;0.000000;,
  0.500000;0.000000;,
  0.500000;0.000000;,
  0.625000;0.000000;,
  0.625000;0.000000;,
  0.750000;0.000000;,
  0.750000;0.000000;,
  0.875000;0.000000;,
  0.875000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.062500;0.000000;,
  0.250000;0.000000;,
  0.187500;0.000000;,
  0.375000;0.000000;,
  0.312500;0.000000;,
  0.500000;0.000000;,
  0.437500;0.000000;,
  0.625000;0.000000;,
  0.562500;0.000000;,
  0.750000;0.000000;,
  0.687500;0.000000;,
  0.875000;0.000000;,
  0.812500;0.000000;,
  1.000000;0.000000;,
  0.937500;0.000000;,
  0.125000;0.562330;,
  0.250000;0.562330;,
  0.000000;0.562330;,
  0.375000;0.562330;,
  1.000000;0.562330;,
  0.875000;0.562330;,
  0.500000;0.562330;,
  0.750000;0.562330;,
  0.625000;0.562330;,
  0.062500;0.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.187500;0.000000;,
  0.250000;0.000000;,
  0.312500;0.000000;,
  0.375000;0.000000;,
  0.437500;0.000000;,
  0.500000;0.000000;,
  0.562500;0.000000;,
  0.625000;0.000000;,
  0.687500;0.000000;,
  0.750000;0.000000;,
  0.812500;0.000000;,
  0.875000;0.000000;,
  0.937500;0.000000;,
  1.000000;0.000000;,
  0.062500;1.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.187500;1.000000;,
  0.250000;1.000000;,
  0.312500;1.000000;,
  0.375000;1.000000;,
  0.437500;1.000000;,
  0.500000;1.000000;,
  0.562500;1.000000;,
  0.625000;1.000000;,
  0.687500;1.000000;,
  0.750000;1.000000;,
  0.812500;1.000000;,
  0.875000;1.000000;,
  0.937500;1.000000;,
  1.000000;1.000000;,
  0.125000;0.633040;,
  0.000000;0.633040;,
  0.250000;0.633040;,
  1.000000;0.633040;,
  0.875000;0.633040;,
  0.375000;0.633040;,
  0.750000;0.633040;,
  0.500000;0.633040;,
  0.625000;0.633040;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.125000;1.000000;,
  0.125000;0.000000;,
  0.250000;1.000000;,
  0.250000;0.000000;,
  0.375000;1.000000;,
  0.375000;0.000000;,
  0.500000;1.000000;,
  0.500000;0.000000;,
  0.625000;1.000000;,
  0.625000;0.000000;,
  0.750000;1.000000;,
  0.750000;0.000000;,
  0.875000;1.000000;,
  0.875000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
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
  0.062500;0.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.187500;0.000000;,
  0.250000;0.000000;,
  0.312500;0.000000;,
  0.375000;0.000000;,
  0.437500;0.000000;,
  0.500000;0.000000;,
  0.562500;0.000000;,
  0.625000;0.000000;,
  0.687500;0.000000;,
  0.750000;0.000000;,
  0.812500;0.000000;,
  0.875000;0.000000;,
  0.937500;0.000000;,
  1.000000;0.000000;,
  0.062500;1.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.187500;1.000000;,
  0.250000;1.000000;,
  0.312500;1.000000;,
  0.375000;1.000000;,
  0.437500;1.000000;,
  0.500000;1.000000;,
  0.562500;1.000000;,
  0.625000;1.000000;,
  0.687500;1.000000;,
  0.750000;1.000000;,
  0.812500;1.000000;,
  0.875000;1.000000;,
  0.937500;1.000000;,
  1.000000;1.000000;,
  0.250000;0.476180;,
  0.375000;0.476180;,
  0.125000;0.476180;,
  0.500000;0.476180;,
  0.000000;0.476180;,
  0.625000;0.476180;,
  1.000000;0.476180;,
  0.875000;0.476180;,
  0.750000;0.476180;,
  0.336840;0.727230;,
  0.336840;1.000000;,
  0.000000;1.000000;,
  0.000000;0.727230;,
  0.000000;0.000000;,
  0.336840;0.000000;,
  0.336840;0.727230;,
  0.745720;0.727230;,
  0.745720;1.000000;,
  1.000000;0.727230;,
  1.000000;1.000000;,
  0.745720;0.000000;,
  1.000000;0.000000;,
  0.745720;0.727230;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.500000;,
  0.500000;0.500000;,
  0.500000;0.500000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.636680;0.500000;,
  0.636680;0.000000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  0.636680;1.000000;,
  1.000000;1.000000;,
  0.636680;0.500000;,
  0.500000;0.500000;,
  0.000000;0.500000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.500000;,
  0.500000;0.195770;,
  1.000000;0.195770;,
  1.000000;0.000000;,
  0.500000;0.000000;,
  0.000000;0.195770;,
  0.000000;0.000000;,
  0.500000;0.195770;;
 }
}