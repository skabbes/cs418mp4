attribute vec2 TextureCoord;
varying vec3 V;
varying vec3 N;

void main(void)
{  
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    // Set up the normals for the vertex shader
    V = vec3(gl_ModelViewMatrix * gl_Vertex);
    N = normalize(gl_NormalMatrix * gl_Normal);

	gl_TexCoord[0].st = TextureCoord;

    // get Sphere map coords
	vec3 r = reflect( V, N );
	float m = 2.0 * sqrt( r.x*r.x + r.y*r.y + (r.z+1.0)*(r.z+1.0) );
	gl_TexCoord[1].st = vec2( r.x/m + 0.5, r.y/m + 0.5);
}
