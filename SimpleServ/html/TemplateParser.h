#ifndef __TEMPLATE_PARSER_H__
#define __TEMPLATE_PARSER_H__

#include "Model.h"
#include <string>

/** 
 * Implements a very basic version of Mustache: https://mustache.github.io/mustache.5.html
 *
 * Only variables and '#' sections are supported for now.
 */
class TemplateParser
{
public:
  std::string Apply(const std::string &docTemplate, const Model &model);
};

#endif //__TEMPLATE_PARSER_H__