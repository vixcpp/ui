/**
 *
 *  @file all.hpp
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
#ifndef VIX_UI_ALL_HPP
#define VIX_UI_ALL_HPP

#include <vix/ui/Version.hpp>

/*
 * Support
 */
#include <vix/ui/support/Error.hpp>
#include <vix/ui/support/Result.hpp>

/*
 * HTML
 */
#include <vix/ui/html/Html.hpp>
#include <vix/ui/html/HtmlAttrs.hpp>
#include <vix/ui/html/HtmlEscape.hpp>
#include <vix/ui/html/HtmlResponse.hpp>

/*
 * Assets
 */
#include <vix/ui/assets/Asset.hpp>
#include <vix/ui/assets/AssetManager.hpp>
#include <vix/ui/assets/AssetManifest.hpp>

/*
 * Core views
 */
#include <vix/ui/core/View.hpp>
#include <vix/ui/core/ViewContext.hpp>
#include <vix/ui/core/ViewData.hpp>
#include <vix/ui/core/ViewResult.hpp>

/*
 * Forms
 */
#include <vix/ui/forms/Field.hpp>
#include <vix/ui/forms/Form.hpp>
#include <vix/ui/forms/ValidationError.hpp>

/*
 * Platform
 */
#include <vix/ui/platform/Platform.hpp>

/*
 * Shell
 */
#include <vix/ui/shell/AppShell.hpp>
#include <vix/ui/shell/ShellConfig.hpp>
#include <vix/ui/shell/ServerProcess.hpp>
#include <vix/ui/shell/ServerReadiness.hpp>

#endif // VIX_UI_ALL_HPP
