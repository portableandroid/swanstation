#include "settings.h"
#include "SimpleIni.h"
#include "YBaseLib/Log.h"
#include <array>
#include <string.h>
Log_SetChannel(Settings);

#ifdef _MSC_VER
#define strcasecmp stricmp
#endif

Settings::Settings() = default;

void Settings::SetDefaults()
{
  gpu_renderer = GPURenderer::HardwareOpenGL;
  gpu_resolution_scale = 1;
  gpu_vsync = true;
  gpu_true_color = true;

  display_linear_filtering = true;

  bios_path = "scph1001.bin";

  memory_card_a_path = "memory_card_a.mcd";
  memory_card_b_path.clear();
}

void Settings::Load(const char* filename)
{
  CSimpleIniA ini(true);
  SI_Error err = ini.LoadFile(filename);
  if (err != SI_OK)
  {
    Log_WarningPrintf("Settings could not be loaded from '%s', defaults will be used.", filename);
    SetDefaults();
    return;
  }

  region = ParseConsoleRegionName(ini.GetValue("Console", "Region", "NTSC-U")).value_or(ConsoleRegion::NTSC_U);

  gpu_renderer = ParseRendererName(ini.GetValue("GPU", "Renderer", "OpenGL")).value_or(GPURenderer::HardwareOpenGL);
  gpu_resolution_scale = static_cast<u32>(ini.GetLongValue("GPU", "ResolutionScale", 1));
  gpu_vsync = static_cast<u32>(ini.GetBoolValue("GPU", "VSync", true));
  gpu_true_color = ini.GetBoolValue("GPU", "TrueColor", false);

  display_linear_filtering = ini.GetBoolValue("Display", "LinearFiltering", true);

  bios_path = ini.GetValue("BIOS", "Path", "scph1001.bin");
  bios_patch_tty_enable = ini.GetBoolValue("BIOS", "PatchTTYEnable", true);
  bios_patch_fast_boot = ini.GetBoolValue("BIOS", "PatchFastBoot", false);

  memory_card_a_path = ini.GetValue("MemoryCard", "CardAPath", "memory_card_a.mcd");
  memory_card_b_path = ini.GetValue("MemoryCard", "CardBPath", "");
}

bool Settings::Save(const char* filename) const
{
  // Load the file first to preserve the comments.
  CSimpleIniA ini;
  SI_Error err = ini.LoadFile(filename);
  if (err != SI_OK)
    ini.Reset();

  ini.SetValue("Console", "Region", GetConsoleRegionName(region));

  ini.SetValue("GPU", "Renderer", GetRendererName(gpu_renderer));
  ini.SetLongValue("GPU", "ResolutionScale", static_cast<long>(gpu_resolution_scale));
  ini.SetBoolValue("GPU", "VSync", gpu_vsync);
  ini.SetBoolValue("GPU", "TrueColor", gpu_true_color);

  ini.SetBoolValue("Display", "LinearFiltering", display_linear_filtering);

  ini.SetValue("BIOS", "Path", bios_path.c_str());
  ini.SetBoolValue("BIOS", "PatchTTYEnable", bios_patch_tty_enable);
  ini.SetBoolValue("BIOS", "PatchFastBoot", bios_patch_fast_boot);

  if (!memory_card_a_path.empty())
    ini.SetValue("MemoryCard", "CardAPath", memory_card_a_path.c_str());
  else
    ini.DeleteValue("MemoryCard", "CardAPath", nullptr);

  if (!memory_card_b_path.empty())
    ini.SetValue("MemoryCard", "CardBPath", memory_card_b_path.c_str());
  else
    ini.DeleteValue("MemoryCard", "CardBPath", nullptr);

  err = ini.SaveFile(filename, false);
  if (err != SI_OK)
  {
    Log_WarningPrintf("Failed to save settings to '%s'.", filename);
    return false;
  }

  return true;
}

static std::array<const char*, 4> s_console_region_names = {{"Auto", "NTSC-J", "NTSC-U", "PAL"}};
static std::array<const char*, 4> s_console_region_display_names = {
  {"Auto-Detect", "NTSC-J (Japan)", "NTSC-U (US)", "PAL (Europe, Australia)"}};

std::optional<ConsoleRegion> Settings::ParseConsoleRegionName(const char* str)
{
  int index = 0;
  for (const char* name : s_console_region_names)
  {
    if (strcasecmp(name, str) == 0)
      return static_cast<ConsoleRegion>(index);

    index++;
  }

  return std::nullopt;
}

const char* Settings::GetConsoleRegionName(ConsoleRegion region)
{
  return s_console_region_names[static_cast<int>(region)];
}

const char* Settings::GetConsoleRegionDisplayName(ConsoleRegion region)
{
  return s_console_region_display_names[static_cast<int>(region)];
}

static std::array<const char*, 3> s_gpu_renderer_names = {{"D3D11", "OpenGL", "Software"}};
static std::array<const char*, 3> s_gpu_renderer_display_names = {
  {"Hardware (D3D11)", "Hardware (OpenGL)", "Software"}};

std::optional<GPURenderer> Settings::ParseRendererName(const char* str)
{
  int index = 0;
  for (const char* name : s_gpu_renderer_names)
  {
    if (strcasecmp(name, str) == 0)
      return static_cast<GPURenderer>(index);

    index++;
  }

  return std::nullopt;
}

const char* Settings::GetRendererName(GPURenderer renderer)
{
  return s_gpu_renderer_names[static_cast<int>(renderer)];
}

const char* Settings::GetRendererDisplayName(GPURenderer renderer)
{
  return s_gpu_renderer_display_names[static_cast<int>(renderer)];
}
