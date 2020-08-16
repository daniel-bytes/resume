#include "TemplateParser.h"
#include "app/Logger.h"

#include <memory>
#include <regex>
#include <stack>
#include <vector>

using std::string;
using std::shared_ptr;
using std::make_shared;
using std::unordered_map;
using std::regex;
using std::regex_token_iterator;
using std::stack;
using std::vector;
using std::size_t;

#define LOGGER "TemplateParser"

using namespace Logger::NdJson;

/**
 * Element: Abstract base class of template hierarchy
 */
class Element
{
public:
  Element(const string &type): _type(type) {}

  virtual string Apply(const Model &model) = 0;

  const string& Type() const { return _type; }

protected:
  string _type;
};

/**
 * ElementContainer: A container of Elements
 */
class ElementContainer
  : public Element
{
public:
  ElementContainer(): Element("ElementContainer") {}

  virtual string Apply(const Model &model) {
    Trace(LOGGER, "ElementContainer::Apply");

    string output;
    for (auto e : _elements) {
      output += e->Apply(model);
    }
    return output;
  }

  void Append(shared_ptr<Element> element) {
    _elements.push_back(element);
  }

private:
  vector<shared_ptr<Element>> _elements;
};

/**
 * Content: An element that just contains static content
 */
class Content
  : public Element
{
public:
  Content(string content): _content(content), Element("Content") {
    Trace(LOGGER, "Content::ctor");
  }

  virtual string Apply(const Model &model) {
    Trace(LOGGER, "Content::Apply");

    return _content;
  }

protected:
  string _content;
};

/**
 * VariableTag: An element that is a Mustache variable tag
 */
class VariableTag
  : public Element
{
public:
  VariableTag(string variable): _variable(variable), Element("VariableTag") {
    Trace(LOGGER, "VariableTag::ctor");
  }

  virtual string Apply(const Model &model) {
    Trace(LOGGER, "VariableTag::Apply");

    return model.Get(_variable);
  }

protected:
  string _variable;
};

/**
 * SectionTag: An element that is a Mustache #section tag
 */
class SectionTag
  : public ElementContainer
{
public:
  SectionTag(string name): _name(name), ElementContainer() {
    Trace(LOGGER, "SectionTag::ctor");
    _type = "SectionTag";
  }

  virtual string Apply(const Model &model) {
    Trace(LOGGER, "SectionTag::Apply");

    if (model.Has(_name) && model.Get(_name).size() > 0) {
      return ElementContainer::Apply(model);
    } else {
      return "";
    }
  }

protected:
  string _name;
};

static regex mustacheRegex("\\{\\{((?!\\}\\})(.|\n))*\\}\\}");

typedef regex_token_iterator<string::iterator> regex_it;
static regex_it rend;

string parseTag(const string &tag) {
  auto end = tag.find_first_not_of("{ ");
  
  if (end != string::npos) {
    auto substr = tag.substr(end);
    end = substr.find_first_of("} ");

    if (end != string::npos) {
      return substr.substr(0, end);
    } else {
      return substr;
    }
  }
  return "";
}

stack<shared_ptr<ElementContainer>> CompileTemplate(const string &docTemplate)
{
  stack<shared_ptr<ElementContainer>> containers;
  containers.push(make_shared<ElementContainer>());
  
  string s = const_cast<string&>(docTemplate);
  regex_it it(s.begin(), s.end(), mustacheRegex);

  string doc = s;
  size_t end = string::npos;

  while (it != rend) {
    auto match = it->str();

    end = doc.find(match);
    if (end != string::npos) {
      auto content = doc.substr(0, end);

      containers.top()->Append(
        make_shared<Content>(Content(content))
      );

      doc = doc.substr(end + match.size());
    }

    auto var = parseTag(match);

    if (var.find_first_of("#") == 0) {
      // new Mustache section
      auto sectionName = var.substr(1);
      auto section = make_shared<SectionTag>(SectionTag(sectionName));
      containers.top()->Append(section);
      containers.push(section);
    } else if (var.find_first_of("/") == 0) {
      // end Mustache section
      // NOTE: for now we are not checking if the tag name matches containers.top() tag
      containers.pop();
    } else {
      // variable
      containers.top()->Append(
        make_shared<VariableTag>(VariableTag(var))
      );
    }
    
    it++;
  }

  end = doc.size();
  if (end != string::npos) {
    auto content = doc.substr(0, end);
    containers.top()->Append(
      make_shared<Content>(Content(content))
    );
  }

  return containers;
}

string 
TemplateParser::Apply(const string &docTemplate, const Model &model)
{
  Trace(LOGGER, "TemplateParser::Apply");

  auto containers = CompileTemplate(docTemplate);
  auto result = containers.top()->Apply(model);

  Trace(LOGGER, "TemplateParser::Apply end");

  return result;
}

// Caching template parser

struct TemplateCache
{
  unordered_map<string, stack<shared_ptr<ElementContainer>>> data;
};

CachingTemplateParser::CachingTemplateParser(): 
  _cache(new TemplateCache) { }

string 
CachingTemplateParser::Apply(const string &docTemplate, const Model &model)
{
  Trace(LOGGER, "CachingTemplateParser::Apply");

  auto cachedData = _cache->data.find(docTemplate);

  if (cachedData != _cache->data.end()) {
    auto result = cachedData->second.top()->Apply(model);

    Trace(LOGGER, "CachingTemplateParser::Apply end with cache");
    return result;
  }

  auto containers = _cache->data[docTemplate] = CompileTemplate(docTemplate);
  auto result = containers.top()->Apply(model);

  Trace(LOGGER, "CachingTemplateParser::Apply end");
  return result;
}