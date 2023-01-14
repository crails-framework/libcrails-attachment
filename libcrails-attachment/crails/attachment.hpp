#ifndef  CRAILS_ATTACHMENT_HPP
# define CRAILS_ATTACHMENT_HPP

# include <crails/params.hpp>

namespace Crails
{
  class Attachment : public std::string
  {
  public:
    Attachment()
    {
    }

    Attachment(const std::string& uid);

    const std::string& get_name() const { return name; }
    const std::string& get_extension() const { return extension; }
    const std::string& get_mimetype() const { return mimetype; }

    void use_uploaded_file(const Crails::Params::File* file);

    virtual void cleanup_files();

    virtual std::string get_store_path() const;
    virtual std::string get_url() const;

  protected:
    std::string get_filepath() const;
    std::string get_filename() const;
    void        new_uid();
    void        generate_uid();
    std::string name, extension, mimetype;
  };
}

#endif
