#ifndef __UPLOAD_HPP__
#define __UPLOAD_HPP__

#include <string>
#include "exception.hpp"

class upload_exception : public zipnshare_exception {
public:
  upload_exception (const std::string& what_arg)
    : zipnshare_exception (what_arg) {}
};


const std::string upload_init (const std::string & url);

void upload_set_metadata (const std::string & url, const std::string & session_key,
			  const std::string & owner_key);

const std::string upload_begin_file (const std::string & url, const std::string & session_key,
				     const std::string & file_name, const std::string & content_type);

void upload_send_file (const std::string & url, const std::string & session_key,
		       const std::string & file_id, const char * data, size_t data_size);

void upload_end_file (const std::string & url, const std::string & session_key,
		      const std::string & file_id);

void upload_end_session (const std::string & url, const std::string & session_key);


#endif
