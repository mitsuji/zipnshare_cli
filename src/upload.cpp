#include "upload.hpp"
#include "utf8.hpp"
#include <curl/curl.h>
#include <sstream>
#include <cstdio>

static const std::string url_encode (CURL * curl, const std::string & in) {
  return std::string (curl_easy_escape(curl, in.c_str(), in.length()));
}

class curl_easy_box {
public:
  curl_easy_box () {
    curl = curl_easy_init();
  }
  virtual ~curl_easy_box () {
    curl_easy_cleanup(curl);
#if 0
    printf("~curl_easy_box\n");
#endif
  }
  CURL * get () const {
    return curl;
  }
private:
  CURL * curl;
};

class curl_mime_box {
public:
  curl_mime_box (CURL * curl) {
    form = curl_mime_init(curl);
  }
  virtual ~curl_mime_box () {
    curl_mime_free(form);
#if 0
    printf("~curl_mime_box\n");
#endif
  }
  curl_mime * get () const {
    return form;
  }
private:
  curl_mime * form;
};

struct write_data_t { std::ostringstream out_body; };

static size_t write_callback(char *buff, size_t size, size_t nitems, void *userdata)
{
  int buff_len = size * nitems;
  struct write_data_t* p_write_data = (struct write_data_t*) userdata;
  p_write_data->out_body.write(buff,buff_len);
  return buff_len;
}

const std::string upload_init (const std::string & url) {
  std::ostringstream out_url;
  out_url << url;
  out_url << "/upload";

  struct write_data_t write_data;   // response body
  
  curl_easy_box _curl_easy_box;
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_CUSTOMREQUEST, "POST");
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_URL, out_url.str().c_str());
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_WRITEDATA, &write_data);

  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_POST, 1);
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_POSTFIELDS, "");
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_POSTFIELDSIZE, 0);

  if (curl_easy_perform(_curl_easy_box.get()) != CURLE_OK) {
    throw upload_exception ("failed to execute curl_easy_perform");
  }

  long http_status = 0;
  if (curl_easy_getinfo (_curl_easy_box.get(), CURLINFO_RESPONSE_CODE, &http_status) != CURLE_OK) {
    throw upload_exception ("failed to execute curl_easy_getinfo");
  }

  if (http_status != 200) {
    std::ostringstream out_err;
    out_err << "http_status:" << http_status << " " << ansi_from_utf8(write_data.out_body.str());
    throw upload_exception (out_err.str());
  }
  
  return ansi_from_utf8(write_data.out_body.str());
}

void upload_set_metadata (const std::string & url, const std::string & session_key,
			  const std::string & owner_key) {
  std::ostringstream out_url;
  out_url << url;
  out_url << "/upload";
  out_url << '/' << session_key;
  out_url << "/set-metadata";

  struct write_data_t write_data;   // response body
  
  curl_easy_box _curl_easy_box;
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_CUSTOMREQUEST, "PUT");
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_URL, out_url.str().c_str());
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_WRITEDATA, &write_data);

  std::string owner_key_utf8 = ansi_to_utf8(owner_key);
  std::string owner_key_utf8_ue = url_encode(_curl_easy_box.get(), owner_key_utf8);
  
  std::ostringstream out_put;
  out_put << "ownerKey=" << owner_key_utf8_ue;
  const std::string & put_data = out_put.str();
  
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_POST, 1);
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_POSTFIELDS, put_data.c_str());
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_POSTFIELDSIZE, put_data.length());
  
  if (curl_easy_perform(_curl_easy_box.get()) != CURLE_OK) {
    throw upload_exception ("failed to execute curl_easy_perform");
  }

  long http_status = 0;
  if (curl_easy_getinfo (_curl_easy_box.get(), CURLINFO_RESPONSE_CODE, &http_status) != CURLE_OK) {
    throw upload_exception ("failed to execute curl_easy_getinfo");
  }

  if (http_status != 200) {
    std::ostringstream out_err;
    out_err << "http_status:" << http_status << " " << ansi_from_utf8(write_data.out_body.str());
    throw upload_exception (out_err.str());
  }

}


const std::string upload_begin_file (const std::string & url, const std::string & session_key,
				     const std::string & file_name, const std::string & content_type) {
  std::ostringstream out_url;
  out_url << url;
  out_url << "/upload";
  out_url << '/' << session_key;
  out_url << "/begin-file";

  struct write_data_t write_data;   // response body
  
  curl_easy_box _curl_easy_box;
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_CUSTOMREQUEST, "PUT");
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_URL, out_url.str().c_str());
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_WRITEDATA, &write_data);

  std::string file_name_utf8 = ansi_to_utf8(file_name);
  std::string content_type_utf8 = ansi_to_utf8(content_type);
  std::string file_name_utf8_ue = url_encode(_curl_easy_box.get(),file_name_utf8);
  std::string content_type_utf8_ue = url_encode(_curl_easy_box.get(),content_type_utf8);
  
  std::ostringstream out_put;
  out_put << "fileName=" << file_name_utf8_ue;
  out_put << '&' << "contentType=" << content_type_utf8_ue;
  const std::string & put_data = out_put.str();
  
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_POST, 1);
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_POSTFIELDS, put_data.c_str());
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_POSTFIELDSIZE, put_data.length());
  
  if (curl_easy_perform(_curl_easy_box.get()) != CURLE_OK) {
    throw upload_exception ("failed to execute curl_easy_perform");
  }

  long http_status = 0;
  if (curl_easy_getinfo (_curl_easy_box.get(), CURLINFO_RESPONSE_CODE, &http_status) != CURLE_OK) {
    throw upload_exception ("failed to execute curl_easy_getinfo");
  }

  if (http_status != 200) {
    std::ostringstream out_err;
    out_err << "http_status:" << http_status << " " << ansi_from_utf8(write_data.out_body.str());
    throw upload_exception (out_err.str());
  }
  
  return ansi_from_utf8(write_data.out_body.str());
}

void upload_send_file (const std::string & url, const std::string & session_key,
		       const std::string & file_id, const char * data, size_t data_size) {
  std::ostringstream out_url;
  out_url << url;
  out_url << "/upload";
  out_url << '/' << session_key;
  out_url << "/send-file";

  struct write_data_t write_data;   // response body
  
  curl_easy_box _curl_easy_box;
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_CUSTOMREQUEST, "PUT");
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_URL, out_url.str().c_str());
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_WRITEDATA, &write_data);

  std::string file_id_utf8 = ansi_to_utf8(file_id);
  
  curl_mime_box _curl_mime_box(_curl_easy_box.get());
  curl_mimepart *field = NULL;

  field = curl_mime_addpart(_curl_mime_box.get());
  curl_mime_name(field, "fileId");
  curl_mime_type(field, NULL);
  curl_mime_data(field, file_id_utf8.c_str(), CURL_ZERO_TERMINATED);
  
  field = curl_mime_addpart(_curl_mime_box.get());
  curl_mime_name(field, "file");
  curl_mime_type(field, "application/octet-stream");
  curl_mime_data(field, data, data_size);

  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_POST, 1);
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_MIMEPOST, _curl_mime_box.get());
 
  if (curl_easy_perform(_curl_easy_box.get()) != CURLE_OK) {
    throw upload_exception ("failed to execute curl_easy_perform");
  }

  long http_status = 0;
  if (curl_easy_getinfo (_curl_easy_box.get(), CURLINFO_RESPONSE_CODE, &http_status) != CURLE_OK) {
    throw upload_exception ("failed to execute curl_easy_getinfo");
  }

  if (http_status != 200) {
    std::ostringstream out_err;
    out_err << "http_status:" << http_status << " " << ansi_from_utf8(write_data.out_body.str());
    throw upload_exception (out_err.str());
  }

}

void upload_end_file (const std::string & url, const std::string & session_key,
		      const std::string & file_id) {
  std::ostringstream out_url;
  out_url << url;
  out_url << "/upload";
  out_url << '/' << session_key;
  out_url << "/end-file";

  struct write_data_t write_data;   // response body
  
  curl_easy_box _curl_easy_box;
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_CUSTOMREQUEST, "PUT");
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_URL, out_url.str().c_str());
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_WRITEDATA, &write_data);

  std::string file_id_utf8 = ansi_to_utf8(file_id);
  std::string file_id_utf8_ue = url_encode(_curl_easy_box.get(),file_id_utf8);

  std::ostringstream out_put;
  out_put << "fileId=" << file_id_utf8_ue;
  const std::string & put_data = out_put.str();
  
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_POST, 1);
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_POSTFIELDS, put_data.c_str());
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_POSTFIELDSIZE, put_data.length());
  
  if (curl_easy_perform(_curl_easy_box.get()) != CURLE_OK) {
    throw upload_exception ("failed to execute curl_easy_perform");
  }

  long http_status = 0;
  if (curl_easy_getinfo (_curl_easy_box.get(), CURLINFO_RESPONSE_CODE, &http_status) != CURLE_OK) {
    throw upload_exception ("failed to execute curl_easy_getinfo");
  }

  if (http_status != 200) {
    std::ostringstream out_err;
    out_err << "http_status:" << http_status << " " << ansi_from_utf8(write_data.out_body.str());
    throw upload_exception (out_err.str());
  }
  
}

void upload_end_session (const std::string & url, const std::string & session_key) {
  std::ostringstream out_url;
  out_url << url;
  out_url << "/upload";
  out_url << '/' << session_key;
  out_url << "/end-session";

  struct write_data_t write_data;   // response body
  
  curl_easy_box _curl_easy_box;
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_CUSTOMREQUEST, "PUT");
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_URL, out_url.str().c_str());
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_WRITEDATA, &write_data);

  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_POST, 1);
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_POSTFIELDS, "");
  curl_easy_setopt(_curl_easy_box.get(), CURLOPT_POSTFIELDSIZE, 0);

  if (curl_easy_perform(_curl_easy_box.get()) != CURLE_OK) {
    throw upload_exception ("failed to execute curl_easy_perform");
  }

  long http_status = 0;
  if (curl_easy_getinfo (_curl_easy_box.get(), CURLINFO_RESPONSE_CODE, &http_status) != CURLE_OK) {
    throw upload_exception ("failed to execute curl_easy_getinfo");
  }

  if (http_status != 200) {
    std::ostringstream out_err;
    out_err << "http_status:" << http_status << " " << ansi_from_utf8(write_data.out_body.str());
    throw upload_exception (out_err.str());
  }
  
}
