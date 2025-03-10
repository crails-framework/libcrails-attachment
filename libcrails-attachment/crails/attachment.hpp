#ifndef  CRAILS_ATTACHMENT_HPP
# define CRAILS_ATTACHMENT_HPP

# include <crails/params.hpp>

namespace Crails
{
  class Attachment : public std::string
  {
  public:
    Attachment() {}
    Attachment(const std::string& uid);
    virtual ~Attachment() {}

    const std::string& get_name() const { return name; }
    const std::string& get_extension() const { return extension; }
    const std::string& get_mimetype() const { return mimetype; }

    void use_uploaded_file(const Crails::Params::File* file);
    void use_filesystem(const std::string&);
    void use_name(const std::string&);
    void link_to(const Attachment&);

    virtual void cleanup_files();

    virtual std::string get_store_path() const;
    virtual std::string get_url() const;
    std::string get_filepath() const;

  protected:
    std::string get_filename() const;
    void        new_uid();
    void        generate_uid();
    std::string name, extension, mimetype;
  };
}

#endif
