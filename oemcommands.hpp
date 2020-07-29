/*
// Copyright (c) 2020 Nuvoton Technology Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/

#pragma once

#include <ipmid/api-types.hpp>

namespace ipmi
{
namespace nuvoton
{

static constexpr NetFn netFnGeneral = netFnOemOne;
static constexpr NetFn netFnPlatform = netFnOemTwo;
static constexpr NetFn netFnApp = netFnOemEight;

namespace general
{
static constexpr Cmd cmdGetStrapPinStatus = 0x02;

} // namespace general

} // namespace nuvoton

} // namespace ipmi
