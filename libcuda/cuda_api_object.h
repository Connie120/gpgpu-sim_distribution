#ifndef __cuda_api_object_h__
#define __cuda_api_object_h__

#include <list>
#include <map>
#include <set>
#include <string>

class cuobjdumpSection;
class cuobjdumpELFSection;
class cuobjdumpPTXSection;
class symbol_table;
class gpgpu_ptx_sim_arg;
class kernel_config;
class kernel_info_t;

typedef std::list<gpgpu_ptx_sim_arg> gpgpu_ptx_sim_arg_list_t;

#ifndef OPENGL_SUPPORT
typedef unsigned long GLuint;
#endif

struct glbmap_entry {
	GLuint m_bufferObj;
	void *m_devPtr;
	size_t m_size;
	struct glbmap_entry *m_next;
};

typedef struct glbmap_entry glbmap_entry_t;


class cuda_runtime_api {
    public:
	cuda_runtime_api() {
	    g_glbmap = NULL;
	}
	// global list
	std::list<cuobjdumpSection*> cuobjdumpSectionList;
	std::list<cuobjdumpSection*> libSectionList;
	std::list<kernel_config> g_cuda_launch_stack;
	std::map<int, bool>fatbin_registered;
	std::map<int, std::string> fatbinmap;
	std::map<std::string, symbol_table*> name_symtab;
	std::map<unsigned long long, size_t> g_mallocPtr_Size;
	//maps sm version number to set of filenames
	std::map<unsigned, std::set<std::string> > version_filename;
	std::map<void *,void **> pinned_memory; //support for pinned memories added
	std::map<void *, size_t> pinned_memory_size;
	glbmap_entry_t* g_glbmap;
	// member function list
	void cuobjdumpInit();
	void extract_code_using_cuobjdump();
	void extract_ptx_files_using_cuobjdump(CUctx_st *context);
	void cuobjdumpParseBinary(unsigned int handle);
	std::list<cuobjdumpSection*> pruneSectionList(CUctx_st *context);
	std::list<cuobjdumpSection*> mergeMatchingSections(std::string identifier);
	std::list<cuobjdumpSection*> mergeSections();
	cuobjdumpELFSection* findELFSection(const std::string identifier);
	cuobjdumpPTXSection* findPTXSection(const std::string identifier);
	void cuobjdumpRegisterFatBinary(unsigned int handle, const char* filename, CUctx_st *context);
	kernel_info_t *gpgpu_cuda_ptx_sim_init_grid( const char *kernel_key,
		gpgpu_ptx_sim_arg_list_t args,
		struct dim3 gridDim,
		struct dim3 blockDim,
		struct CUctx_st* context );
};
#endif /* __cuda_api_object_h__ */
