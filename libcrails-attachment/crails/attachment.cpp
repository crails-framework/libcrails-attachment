#include "attachment.hpp"
#include <crails/utils/random_string.hpp>
#include <crails/utils/split.hpp>
#include <crails/utils/join.hpp>
#include <filesystem>
#include <crails/logger.hpp>
#include <crails/server.hpp>

using namespace std;
using namespace Crails;

Attachment::Attachment(const string& uid) : std::string(uid)
{
  list<string> parts = split(uid, '.');

  if (parts.size() >= 3)
  {
    auto part = parts.rbegin();

    extension = *part++;
    mimetype  = *part;
    for (int i = 0 ; i < 2 ; ++i)
      parts.erase(std::next(parts.rbegin()).base());
    name = join(parts, '.');
  }
  else
    name = uid;
}

static string find_extension_in_filename(const string& value)
{
  list<string> parts = split(value, '.');

  if (parts.size() > 1)
    return *parts.rbegin();
  return ".blob";
}

void Attachment::use_uploaded_file(const Crails::Params::File* file)
{
  if (file)
  {
    if (length() > 0)
      cleanup_files();
    extension = find_extension_in_filename(file->name);
    mimetype = file->mimetype;
    generate_uid();
    logger << Logger::Info << "storing attachment at " << get_filepath() << Logger::endl;
    filesystem::create_directories(get_store_path());
    filesystem::copy_file(file->temporary_path, get_filepath());
  }
  else
    std::string::operator=("");
}

void Attachment::cleanup_files()
{
  auto path = get_filepath();

  if (filesystem::is_regular_file(path))
    filesystem::remove(path);
}

void Attachment::new_uid()
{
  name = "";
  generate_uid();
}

void Attachment::generate_uid()
{
  std::string       charset("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
  std::stringstream stream;

  if (name.length())
    stream << name;
  else
  {
    stream << std::time(0) << '_';
    stream << generate_random_string(charset, 5);
  }
  name = stream.str();
  std::string::operator=(stream.str() + '.' + mimetype + '.' + extension);
}

std::string Attachment::get_store_path() const
{
  auto public_paths = Server::singleton::require().get_public_paths();

  if (!public_paths.size())
    throw boost_ext::runtime_error("No public paths found for storing attachments.");
  return *public_paths.begin() + "/attachments";
}

std::string Attachment::get_url() const
{
  auto public_paths = Server::singleton::require().get_public_paths();
  std::string store_path = get_store_path();
  std::string url;

  for (auto public_path : public_paths)
  {
    if (store_path.find(public_path) == 0)
    {
      url = get_filepath().substr(public_path.length());
      break ;
    }
  }
  if (!url.length())
    throw boost_ext::runtime_error("Attachment does not have any URL.");
  if (url[0] == '/')
    return url;
  return '/' + url;
}

std::string Attachment::get_filepath() const
{
  return get_store_path() + '/' + get_filename();
}

std::string Attachment::get_filename() const
{
  return name + '.' + extension;
}

void Attachment::use_name(const std::string& value)
{
  const auto old_path = get_filepath();

  name = value;
  filesystem::rename(old_path, get_filepath());
}
