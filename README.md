# libcrails-attachment

The `Crails::Attachment` class can help you attach stored files to your application models.

## Usage

The following example shows how you can store files your Crails server receives by using the Attachment class:

```
#include <crails/router.hpp>
#include <crails/attachment.hpp>
#include <iostream>

// The following global must be declared in your crails application:
const std::string Crails::Attachment::default_store_path = "/tmp";

void Crails::Router::initialize()
{
  match("GET", "/upload", [](Crails::Context& context, std::function<void()> callback)
  {
    Crails::Attachment attachment;
    Crails::Params::File* file = context.params["multipart_field_name"];

    if (file)
    {
      attachment.use_uploaded_file(file);
      // The file is now stored on hard drive. We will now display its absolute path:
      std::cout << "File stored at: " << attachment.get_path() << std::endl;
      // The attachment class stores an UID that you may use later to find back the file: 
      std::cout << "File UID: " << attachment << std::endl;
    }
    else
      std::cout << "No file sent" << std::endl;
    callback();
  });
}
```

You can also override the default storepath by inheriting from Attachment:

```
class MyAttachment : public Crails::Attachment
{
  std::string get_default_store_path() const override
  {
    return "/srv/file_storage";
  }
};
```
