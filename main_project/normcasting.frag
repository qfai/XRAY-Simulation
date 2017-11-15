#version 400
// 杜绝声明未使用的变量，避免bug的产生。


in vec3 EntryPoint;
in vec4 ExitPointCoord;

uniform sampler2D EntryPoints;
uniform sampler2D ExitPoints;
uniform sampler3D VolumeTex;
uniform sampler1D TransferFunc;  
uniform float     StepSize;
uniform vec3      DimensionSize;
//uniform vec2      ScreenSize;
layout (location = 0) out vec4 FragColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vec2 exitFragCoord = (ExitPointCoord.xy / ExitPointCoord.w + 1.0)/2.0;
    vec3 EntryCoord = texture(EntryPoints, exitFragCoord).xyz;
    vec3 exitPoint  = texture(ExitPoints, exitFragCoord).xyz;
    if (EntryCoord == exitPoint)
        //background need no raycasting
        discard;
    vec3 dir = exitPoint - EntryCoord;
    float len = length(dir); // the length from front to back is calculated and used to terminate the ray
    vec3 deltaDir = normalize(dir) * StepSize;
    float deltaDirLen = length(deltaDir);
    //float steps=1/StepSize;

    vec4 colorAcum = vec4(0.0); // The dest color
    float alphaAcum = 0.0;                // The  dest alpha for blending

   
    float lengthAcum = 0.0;
    vec4 colorSample; // The src color 
    float alphaSample; // The src alpha
    // backgroundColor
    vec4 bgColor = vec4(0.8, 0.8, 0.8, 0.0);
    vec3 voxelCoord=EntryCoord;

     for(int i = 0; i < 1600; i++)
     {
        //voxelCoord=EntryCoord+i*deltaDir;
       float intensity = texture(VolumeTex, voxelCoord).y;
        //calc norm here
        vec4 xyzw;
        if(voxelCoord.x==0)
            xyzw.y=texture(VolumeTex,vec3(voxelCoord.x+0.004,voxelCoord.y,voxelCoord.z)).x-intensity;
        else if(voxelCoord.x==DimensionSize.x)
            xyzw.y=intensity-texture(VolumeTex,vec3(voxelCoord.x-0.004,voxelCoord.y,voxelCoord.z)).x;
        else
            xyzw.y=(texture(VolumeTex,vec3(voxelCoord.x+0.004,voxelCoord.y,voxelCoord.z)).x-texture(VolumeTex,vec3(voxelCoord.x-0.004,voxelCoord.y,voxelCoord.z))).x/2.0;
        if(voxelCoord.y==0)
            xyzw.z=texture(VolumeTex,vec3(voxelCoord.x,voxelCoord.y+0.004,voxelCoord.z)).x-intensity;
        else if(voxelCoord.y==DimensionSize.y)
            xyzw.z=intensity-texture(VolumeTex,vec3(voxelCoord.x,voxelCoord.y-0.004,voxelCoord.z)).x;
        else
            xyzw.z=(texture(VolumeTex,vec3(voxelCoord.x,voxelCoord.y+0.004,voxelCoord.z)).x-texture(VolumeTex,vec3(voxelCoord.x,voxelCoord.y-0.004,voxelCoord.z))).x/2.0;
        if(voxelCoord.z==0)
            xyzw.w=texture(VolumeTex,vec3(voxelCoord.x,voxelCoord.y,voxelCoord.z+0.004)).x-intensity;
        else if(voxelCoord.z==DimensionSize.z)
            xyzw.w=intensity-texture(VolumeTex,vec3(voxelCoord.x,voxelCoord.y,voxelCoord.z-0.004)).x;
        else
            xyzw.w=(texture(VolumeTex,vec3(voxelCoord.x,voxelCoord.y,voxelCoord.z+0.004)).x-texture(VolumeTex,vec3(voxelCoord.x,voxelCoord.y,voxelCoord.z-0.004))).x/2.0;
  //  }
   // FragColor=xyzw;
        vec3 norm =  normalize(xyzw.yzw);
        //vec4 currentCoord = mvpMat * vec4(voxelCoord, 1);

        vec3 lightDir = normalize(lightPos - voxelCoord);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;
        float ambientStrength = 0.1f;
        vec3 ambient = ambientStrength * lightColor;

        float specularStrength = 0.5f;
        vec3 viewDir = normalize(viewPos - voxelCoord);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
        vec3 specular = specularStrength * spec * lightColor;

        colorSample = texture(TransferFunc, intensity);

        colorSample.rgb = (ambient + diffuse + specular) * colorSample.rgb;
        
        // modulate the value of colorSample.a
        // front-to-back integration
        if (colorSample.a > 0.0) {
            // accomodate for variable sampling rates (base interval defined by mod_compositing.frag)
            colorSample.a = 1.0 - pow(1.0 - colorSample.a, StepSize*200.0f);
            colorAcum.rgb += (1.0 - colorAcum.a) * colorSample.rgb * colorSample.a;
            colorAcum.a += (1.0 - colorAcum.a) * colorSample.a;
        }
        voxelCoord += deltaDir;
        lengthAcum += deltaDirLen;
        if (lengthAcum >= len)
        {   
            colorAcum.rgb = colorAcum.rgb*colorAcum.a + (1 - colorAcum.a) * bgColor.rgb;        
            break;  // terminate if opacity > 1 or the ray is outside the volume    
        } else if (colorAcum.a > 1.0) {
            colorAcum.a = 1.0;
            break;
        }
    }
    FragColor = colorAcum;
   
}