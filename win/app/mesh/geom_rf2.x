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
 74;
 -0.01513;0.00433;-0.05033;,
 -0.01527;-0.01601;-0.05033;,
 -0.01689;-0.01729;-0.04583;,
 -0.01685;0.00433;-0.04583;,
 -0.01743;-0.01772;-0.04061;,
 -0.01737;0.00433;-0.04061;,
 -0.01820;-0.01630;-0.01397;,
 -0.01802;0.00433;-0.01397;,
 -0.01765;-0.01146;0.00019;,
 -0.01795;0.00798;-0.00496;,
 -0.01700;-0.00367;0.00804;,
 -0.01763;0.00839;0.00295;,
 0.01763;0.00886;0.00186;,
 0.01704;-0.00395;0.00809;,
 0.01766;-0.01146;0.00019;,
 0.01800;0.00798;-0.00496;,
 0.01826;-0.01630;-0.01397;,
 0.01787;0.00470;-0.01397;,
 0.01744;-0.01772;-0.04061;,
 0.01712;0.00433;-0.04061;,
 0.01712;-0.01734;-0.04583;,
 0.01677;0.00433;-0.04578;,
 0.01539;-0.01601;-0.05033;,
 0.01539;0.00433;-0.05033;,
 0.00945;-0.01513;-0.05502;,
 0.00945;0.00433;-0.05498;,
 -0.00974;0.00433;-0.05493;,
 -0.00974;-0.01497;-0.05473;,
 -0.01527;-0.01601;-0.05033;,
 -0.01119;-0.01896;-0.04956;,
 -0.01370;-0.01971;-0.04583;,
 -0.01689;-0.01729;-0.04583;,
 -0.01487;-0.02016;-0.04061;,
 -0.01743;-0.01772;-0.04061;,
 -0.01569;-0.01904;-0.01397;,
 -0.01820;-0.01630;-0.01397;,
 -0.01565;-0.01495;0.00019;,
 -0.01765;-0.01146;0.00019;,
 -0.01519;-0.00835;0.00923;,
 -0.01700;-0.00367;0.00804;,
 0.01704;-0.00395;0.00809;,
 0.01508;-0.00813;0.00923;,
 0.01555;-0.01467;0.00038;,
 0.01766;-0.01146;0.00019;,
 0.01574;-0.01900;-0.01392;,
 0.01826;-0.01630;-0.01397;,
 0.01484;-0.02013;-0.04064;,
 0.01744;-0.01772;-0.04061;,
 0.01376;-0.01959;-0.04573;,
 0.01712;-0.01734;-0.04583;,
 0.01071;-0.01887;-0.05001;,
 0.01539;-0.01601;-0.05033;,
 0.00639;-0.01836;-0.05238;,
 0.00945;-0.01513;-0.05502;,
 -0.00814;-0.01845;-0.05177;,
 -0.00974;-0.01497;-0.05473;,
 0.00739;-0.02119;-0.04761;,
 -0.00810;-0.02101;-0.04733;,
 0.01190;-0.02208;-0.04078;,
 -0.01185;-0.02216;-0.04078;,
 0.01251;-0.02064;-0.01397;,
 -0.01279;-0.02086;-0.01397;,
 0.01252;-0.01637;0.00019;,
 -0.01241;-0.01644;0.00019;,
 0.01184;-0.01067;0.00838;,
 -0.01169;-0.01038;0.00878;,
 -0.00810;-0.02101;-0.04733;,
 0.00739;-0.02119;-0.04761;,
 0.01190;-0.02208;-0.04078;,
 -0.01185;-0.02216;-0.04078;,
 0.01251;-0.02064;-0.01397;,
 -0.01279;-0.02086;-0.01397;,
 0.01252;-0.01637;0.00019;,
 -0.01241;-0.01644;0.00019;;
 
 40;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 4;9,8,10,11;,
 4;12,13,14,15;,
 4;15,14,16,17;,
 4;17,16,18,19;,
 4;19,18,20,21;,
 4;21,20,22,23;,
 4;23,22,24,25;,
 4;26,27,1,0;,
 4;28,29,30,31;,
 4;31,30,32,33;,
 4;33,32,34,35;,
 4;35,34,36,37;,
 4;37,36,38,39;,
 4;40,41,42,43;,
 4;43,42,44,45;,
 4;45,44,46,47;,
 4;47,46,48,49;,
 4;49,48,50,51;,
 4;51,50,52,53;,
 4;53,52,54,55;,
 4;55,54,29,28;,
 4;52,50,48,56;,
 4;54,52,56,57;,
 4;29,54,57,30;,
 4;56,48,46,58;,
 4;30,57,59,32;,
 4;58,46,44,60;,
 4;32,59,61,34;,
 4;60,44,42,62;,
 4;34,61,63,36;,
 4;62,42,41,64;,
 4;36,63,65,38;,
 4;66,67,68,69;,
 4;69,68,70,71;,
 4;71,70,72,73;,
 4;73,72,64,65;;
 
 MeshMaterialList {
  2;
  40;
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
   0.267000;0.267000;0.267000;1.000000;;
   5.000000;
   0.405000;0.405000;0.405000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.784000;0.784000;0.784000;1.000000;;
   5.000000;
   0.330000;0.330000;0.330000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  56;
  -0.814723;0.001139;-0.579849;,
  -0.973944;0.003066;-0.226767;,
  -0.997936;0.003920;-0.064091;,
  -0.999997;0.002226;-0.001166;,
  -0.999087;-0.007682;0.042037;,
  -0.998123;-0.014058;0.059603;,
  0.997754;-0.007699;0.066543;,
  0.999169;-0.004088;0.040550;,
  0.999937;0.008026;-0.007787;,
  0.998768;0.015884;-0.047015;,
  0.980710;0.011659;-0.195118;,
  0.816048;0.004490;-0.577967;,
  0.618729;0.000740;-0.785604;,
  -0.636343;-0.001803;-0.771404;,
  -0.379206;-0.774208;-0.506760;,
  -0.435894;-0.721894;-0.537463;,
  -0.466608;-0.820325;-0.330672;,
  -0.579715;-0.756100;-0.303714;,
  -0.589293;-0.801614;-0.100745;,
  -0.679892;-0.729214;-0.077422;,
  -0.639404;-0.758504;0.125832;,
  -0.757879;-0.643784;0.105648;,
  -0.647763;-0.669401;0.363737;,
  -0.832790;-0.476558;0.281696;,
  -0.642310;-0.590854;0.488189;,
  -0.858329;-0.365624;0.359987;,
  0.643155;-0.590371;0.487662;,
  0.831718;-0.400743;0.384253;,
  0.639970;-0.674541;0.368012;,
  0.809205;-0.504052;0.301858;,
  0.620445;-0.773183;0.131285;,
  0.744261;-0.658202;0.113338;,
  0.572877;-0.814033;-0.095723;,
  0.661801;-0.746477;-0.069215;,
  0.453252;-0.827228;-0.332049;,
  0.547726;-0.783243;-0.294155;,
  0.353289;-0.786496;-0.506568;,
  0.396572;-0.750903;-0.528087;,
  0.138574;-0.787073;-0.601094;,
  0.150130;-0.692822;-0.705307;,
  -0.168198;-0.782500;-0.599503;,
  -0.193233;-0.674989;-0.712075;,
  0.233391;-0.877335;-0.419300;,
  -0.239274;-0.880805;-0.408572;,
  0.475743;-0.871203;-0.121133;,
  -0.489742;-0.863176;-0.122804;,
  0.478713;-0.865841;0.145442;,
  -0.503089;-0.852395;0.142561;,
  0.430130;-0.803559;0.411438;,
  -0.410970;-0.810421;0.417519;,
  0.205690;-0.792223;0.574520;,
  -0.179092;-0.797109;0.576665;,
  -0.005463;-0.987743;-0.155995;,
  0.000248;-0.998609;-0.052718;,
  0.005690;-0.984803;0.173582;,
  0.003148;-0.898561;0.438837;;
  40;
  4;0,0,1,1;,
  4;1,1,2,2;,
  4;2,2,3,3;,
  4;3,3,4,4;,
  4;4,4,5,5;,
  4;6,6,7,7;,
  4;7,7,8,8;,
  4;8,8,9,9;,
  4;9,9,10,10;,
  4;10,10,11,11;,
  4;11,11,12,12;,
  4;13,13,0,0;,
  4;15,14,16,17;,
  4;17,16,18,19;,
  4;19,18,20,21;,
  4;21,20,22,23;,
  4;23,22,24,25;,
  4;27,26,28,29;,
  4;29,28,30,31;,
  4;31,30,32,33;,
  4;33,32,34,35;,
  4;35,34,36,37;,
  4;37,36,38,39;,
  4;39,38,40,41;,
  4;41,40,14,15;,
  4;38,36,34,42;,
  4;40,38,42,43;,
  4;14,40,43,16;,
  4;42,34,32,44;,
  4;16,43,45,18;,
  4;44,32,30,46;,
  4;18,45,47,20;,
  4;46,30,28,48;,
  4;20,47,49,22;,
  4;48,28,26,50;,
  4;22,49,51,24;,
  4;52,52,53,53;,
  4;53,53,54,54;,
  4;54,54,55,55;,
  4;55,55,50,51;;
 }
}
