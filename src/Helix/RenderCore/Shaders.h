#ifndef SHADERS_H
#define SHADERS_H

#include <string>
#include <map>

struct HXVertexDecl;

struct HXShader
{
	HXShader(const std::string &name) : m_decl(NULL), m_pEffect(NULL), m_loading(false), m_needsProcessing(false) 
	{
		m_shaderName = name;
	}

	std::string		m_shaderName;
	HXVertexDecl *	m_decl;
	ID3D10Effect *	m_pEffect;

	union {
		unsigned long	flags;
		struct {
			bool m_loading : 1;
			bool m_needsProcessing : 1;
		}; 
	};
}; 

void		HXInitializeShaders();
HXShader *	HXGetShaderByName(const std::string &shaderName);
HXShader *	HXLoadShader(const std::string &shaderName);
void		HXSetSharedParameter(const std::string &paramName, D3DXMATRIX &matrix);


#endif // SHADERS_H