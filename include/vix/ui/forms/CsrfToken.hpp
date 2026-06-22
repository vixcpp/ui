/**
 *
 *  @file CsrfToken.hpp
 *  @author Gaspard Kirira
 *
 *  Copyright 2025, Gaspard Kirira.
 *  All rights reserved.
 *  https://github.com/vixcpp/vix
 *
 *  Use of this source code is governed by a MIT license
 *  that can be found in the License file.
 *
 *  Vix.cpp
 *
 */
#ifndef VIX_UI_FORMS_CSRF_TOKEN_HPP
#define VIX_UI_FORMS_CSRF_TOKEN_HPP

#include <string>

#include <vix/ui/html/HtmlAttrs.hpp>

namespace vix::ui
{
  /**
   * @brief CSRF token helper for server-rendered forms.
   *
   * CsrfToken stores the field name, token value, optional header name, and
   * custom attributes needed to render a CSRF hidden input or metadata tag.
   *
   * It does not generate tokens by itself. Applications should generate,
   * sign, validate, and rotate tokens in their own security layer.
   */
  class CsrfToken
  {
  public:
    /**
     * @brief Construct an empty CSRF token using the default field name.
     */
    CsrfToken() = default;

    /**
     * @brief Construct a CSRF token with a value.
     *
     * @param value Token value.
     */
    explicit CsrfToken(std::string value);

    /**
     * @brief Construct a CSRF token with a field name and value.
     *
     * @param name Hidden field name.
     * @param value Token value.
     */
    CsrfToken(std::string name, std::string value);

    /**
     * @brief Create a CSRF token.
     *
     * @param value Token value.
     * @return CSRF token.
     */
    [[nodiscard]] static CsrfToken make(std::string value = {});

    /**
     * @brief Create a named CSRF token.
     *
     * @param name Hidden field name.
     * @param value Token value.
     * @return CSRF token.
     */
    [[nodiscard]] static CsrfToken named(
        std::string name,
        std::string value = {});

    /**
     * @brief Set the hidden field name.
     *
     * @param name Hidden field name.
     * @return This CSRF token.
     */
    CsrfToken &set_name(std::string name);

    /**
     * @brief Set the token value.
     *
     * @param value Token value.
     * @return This CSRF token.
     */
    CsrfToken &set_value(std::string value);

    /**
     * @brief Set the HTTP header name associated with this token.
     *
     * This is optional metadata for clients that submit CSRF tokens through
     * request headers.
     *
     * @param header_name Header name.
     * @return This CSRF token.
     */
    CsrfToken &set_header_name(std::string header_name);

    /**
     * @brief Set or replace a custom hidden input attribute.
     *
     * @param name Attribute name.
     * @param value Attribute value.
     * @return This CSRF token.
     */
    CsrfToken &set_attr(std::string name, std::string value);

    /**
     * @brief Set or remove a custom boolean hidden input attribute.
     *
     * @param name Attribute name.
     * @param enabled Whether the attribute should be present.
     * @return This CSRF token.
     */
    CsrfToken &set_bool_attr(std::string name, bool enabled = true);

    /**
     * @brief Get the hidden field name.
     *
     * @return Hidden field name.
     */
    [[nodiscard]] const std::string &name() const noexcept;

    /**
     * @brief Get the token value.
     *
     * @return Token value.
     */
    [[nodiscard]] const std::string &value() const noexcept;

    /**
     * @brief Get the associated HTTP header name.
     *
     * @return Header name.
     */
    [[nodiscard]] const std::string &header_name() const noexcept;

    /**
     * @brief Access custom hidden input attributes.
     *
     * @return Immutable attributes.
     */
    [[nodiscard]] const HtmlAttrs &attrs() const noexcept;

    /**
     * @brief Access custom hidden input attributes.
     *
     * @return Mutable attributes.
     */
    [[nodiscard]] HtmlAttrs &attrs() noexcept;

    /**
     * @brief Check whether the token has a hidden field name.
     *
     * @return True if name is not empty.
     */
    [[nodiscard]] bool has_name() const noexcept;

    /**
     * @brief Check whether the token has a value.
     *
     * @return True if value is not empty.
     */
    [[nodiscard]] bool has_value() const noexcept;

    /**
     * @brief Check whether the token has a header name.
     *
     * @return True if header name is not empty.
     */
    [[nodiscard]] bool has_header_name() const noexcept;

    /**
     * @brief Check whether the token value is empty.
     *
     * @return True if value is empty.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Render the token as a hidden input.
     *
     * @return Hidden input HTML.
     */
    [[nodiscard]] std::string render() const;

    /**
     * @brief Render the token as a meta tag.
     *
     * The generated tag contains:
     * - name
     * - content
     * - data-header when a header name is set
     *
     * @param meta_name Meta tag name.
     * @return Meta tag HTML.
     */
    [[nodiscard]] std::string render_meta(
        std::string meta_name = "csrf-token") const;

  private:
    /**
     * @brief Hidden field name.
     */
    std::string name_{"_token"};

    /**
     * @brief Token value.
     */
    std::string value_;

    /**
     * @brief Optional header name.
     */
    std::string header_name_{"X-CSRF-Token"};

    /**
     * @brief Custom hidden input attributes.
     */
    HtmlAttrs attrs_;
  };

} // namespace vix::ui

#endif // VIX_UI_FORMS_CSRF_TOKEN_HPP
