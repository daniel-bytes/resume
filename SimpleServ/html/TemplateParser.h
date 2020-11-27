#ifndef __TEMPLATE_PARSER_H__
#define __TEMPLATE_PARSER_H__

#include "Model.h"
#include <memory>
#include <string>

/** 
 * Implements a very basic version of Mustache: https://mustache.github.io/mustache.5.html
 *
 * Only variables and '#' sections are supported for now.
 */
class TemplateParser
{
public:
  /**
   * Applies the model to the template supplied, returning a converted document
   */
  virtual std::string Apply(const std::string &docTemplate, const Model &model);
};

/**
 * template cache type
 */
struct TemplateCache;

/**
 * Caching template parser: stores compiled templates mapped by doc template
 */
class CachingTemplateParser
  : public TemplateParser
{
public:
  CachingTemplateParser();
  
  /**
   * Applies the model to the template supplied, returning a converted document
   */
  virtual std::string Apply(const std::string &docTemplate, const Model &model);

private:
  std::unique_ptr<TemplateCache> _cache;
};

#endif //__TEMPLATE_PARSER_H__