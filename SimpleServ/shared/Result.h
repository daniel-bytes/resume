#ifndef __RESULT_H__
#define __RESULT_H__

#include <optional>

/**
 * A wrapper around a value that can be either a success or an error.
 * Used by functions that would like to return a possible error without throwing.
 * 
 * Simple example usage, which either returns the underlying value or 
 * throws the underlying error:
 * 
 *     auto result = ResultYieldingFunction().Handle(
 *       [](auto r) { return r },
 *       [](auto e) { throw e }
 *     );
 */
template<class TValue, class TError>
class Result
{
public:
  Result(const TValue &value)
    : _value(value) {}

  Result(const TError &error)
    : _error(error) {}

public:
  /**
   * Handles the result by taking 2 callbacks.
   * If a TValue is present, it is used as an argument to the first callback,
   * otherwise the TError is used as an argument to the second callback.
   * 
   * Both callbacks are expected to return a TValue (or possibly throw instead).
   */
  template<typename OnValue, typename OnError>
  TValue Handle(OnValue &&onValue, OnError &&onError) {
    if (_value.has_value()) {
      return onValue(_value.value());
    }

    return onError(_error.value());
  }

private:
  std::optional<TValue> _value;
  std::optional<TError> _error;
};

#endif //__RESULT_H__