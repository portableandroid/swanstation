#pragma once
#include "common/log.h"
#include "common/string.h"
#include "types.h"
#include <array>
#include <optional>
#include <string>
#include <vector>

class SettingsInterface
{
public:
  virtual ~SettingsInterface();

  virtual int GetIntValue(const char* section, const char* key, int default_value = 0) = 0;
  virtual float GetFloatValue(const char* section, const char* key, float default_value = 0.0f) = 0;
  virtual bool GetBoolValue(const char* section, const char* key, bool default_value = false) = 0;
  virtual std::string GetStringValue(const char* section, const char* key, const char* default_value = "") = 0;

  virtual std::vector<std::string> GetStringList(const char* section, const char* key) = 0;
};

struct SettingInfo
{
  enum class Type
  {
    Boolean,
    Integer,
    Float,
    String,
    Path,
  };

  Type type;
  const char* key;
  const char* visible_name;
  const char* description;
  const char* default_value = nullptr;
  const char* min_value = nullptr;
  const char* max_value = nullptr;
  const char* step_value = nullptr;

  const char* StringDefaultValue() const;
  bool BooleanDefaultValue() const;
  s32 IntegerDefaultValue() const;
  s32 IntegerMinValue() const;
  s32 IntegerMaxValue() const;
  s32 IntegerStepValue() const;
  float FloatDefaultValue() const;
  float FloatMinValue() const;
  float FloatMaxValue() const;
  float FloatStepValue() const;
};

struct Settings
{
  Settings();

  ConsoleRegion region = ConsoleRegion::Auto;

  bool audio_fast_hook = true;

  CPUExecutionMode cpu_execution_mode = CPUExecutionMode::Interpreter;
  u32 cpu_overclock_numerator = 1;
  u32 cpu_overclock_denominator = 1;
  bool cpu_overclock_enable = false;
  bool cpu_overclock_active = false;
  bool cpu_recompiler_memory_exceptions = false;
  bool cpu_recompiler_block_linking = true;
  bool cpu_recompiler_icache = false;
  CPUFastmemMode cpu_fastmem_mode = CPUFastmemMode::Disabled;
  bool cpu_fastmem_rewrite = false;

  bool apply_game_settings = true;
  bool disable_all_enhancements = false;

  bool rewind_enable = false;
  float rewind_save_frequency = 10.0f;
  u32 rewind_save_slots = 10;
  u32 runahead_frames = 0;

  GPURenderer gpu_renderer = GPURenderer::Software;
  u32 gpu_resolution_scale = 1;
  u32 gpu_multisamples = 1;
  bool gpu_use_thread = true;
  bool gpu_use_software_renderer_for_readbacks = false;
  bool gpu_per_sample_shading = false;
  bool gpu_true_color = false;
  bool gpu_scaled_dithering = false;
  GPUTextureFilter gpu_texture_filter = GPUTextureFilter::Nearest;
  GPUDownsampleMode gpu_downsample_mode = GPUDownsampleMode::Disabled;
  bool gpu_disable_interlacing = true;
  bool gpu_force_ntsc_timings = false;
  bool gpu_widescreen_hack = false;
  bool gpu_pgxp_enable = false;
  bool gpu_pgxp_culling = true;
  bool gpu_pgxp_texture_correction = true;
  bool gpu_pgxp_color_correction = false;
  bool gpu_pgxp_vertex_cache = false;
  bool gpu_pgxp_cpu = false;
  bool gpu_pgxp_preserve_proj_fp = false;
  bool gpu_pgxp_depth_buffer = false;
  DisplayCropMode display_crop_mode = DisplayCropMode::None;
  DisplayAspectRatio display_aspect_ratio = DisplayAspectRatio::Auto;
  u16 display_aspect_ratio_custom_numerator = 0;
  u16 display_aspect_ratio_custom_denominator = 0;
  s16 display_active_start_offset = 0;
  s16 display_active_end_offset = 0;
  s8 display_line_start_offset = 0;
  s8 display_line_end_offset = 0;
  bool display_force_4_3_for_24bit = false;
  bool gpu_24bit_chroma_smoothing = false;
  bool display_show_osd_messages = true;
  bool display_show_enhancements = false;
  float gpu_pgxp_tolerance = -1.0f;
  float gpu_pgxp_depth_clear_threshold = 300.0f / 4096.0f;

  u8 cdrom_readahead_sectors = DEFAULT_CDROM_READAHEAD_SECTORS;
  bool cdrom_region_check = false;
  bool cdrom_load_image_to_ram = false;
  bool cdrom_precache_chd = false;
  bool cdrom_mute_cd_audio = false;
  u32 cdrom_read_speedup = 1;
  u32 cdrom_seek_speedup = 1;

  u32 audio_buffer_size = 2048;

  bool use_old_mdec_routines = true;

  // timing hacks section
  TickCount dma_max_slice_ticks = 1000;
  TickCount dma_halt_ticks = 100;
  u32 gpu_fifo_size = 128;
  TickCount gpu_max_run_ahead = 128;

  // texture replacements
  struct TextureReplacementSettings
  {
    bool enable_vram_write_replacements = false;
    bool preload_textures = false;

    bool dump_vram_writes = false;
    bool dump_vram_write_force_alpha_channel = true;
    u32 dump_vram_write_width_threshold = 128;
    u32 dump_vram_write_height_threshold = 128;

    ALWAYS_INLINE bool AnyReplacementsEnabled() const { return enable_vram_write_replacements; }

    ALWAYS_INLINE bool ShouldDumpVRAMWrite(u32 width, u32 height)
    {
      return dump_vram_writes && width >= dump_vram_write_width_threshold && height >= dump_vram_write_height_threshold;
    }
  } texture_replacements;

  // TODO: Controllers, memory cards, etc.

  bool bios_patch_tty_enable = false;
  bool bios_patch_fast_boot = false;
  bool enable_8mb_ram = false;

  std::array<ControllerType, NUM_CONTROLLER_AND_CARD_PORTS> controller_types{};

  u32 controller_analog_combo = 1;
  bool controller_enable_rumble = true;
  bool controller_show_crosshair = true;

  std::array<MemoryCardType, NUM_CONTROLLER_AND_CARD_PORTS> memory_card_types{};
  std::array<std::string, NUM_CONTROLLER_AND_CARD_PORTS> memory_card_paths{};
  std::string memory_card_directory;
  bool memory_card_use_playlist_title = true;

  MultitapMode multitap_mode = MultitapMode::Disabled;

  std::array<TinyString, NUM_CONTROLLER_AND_CARD_PORTS> GeneratePortLabels() const;

  LogLevel log_level = LogLevel::Info;

  ALWAYS_INLINE bool IsUsingCodeCache() const { return (cpu_execution_mode != CPUExecutionMode::Interpreter); }
  ALWAYS_INLINE bool IsUsingRecompiler() const { return (cpu_execution_mode == CPUExecutionMode::Recompiler); }
  ALWAYS_INLINE bool IsUsingSoftwareRenderer() const { return (gpu_renderer == GPURenderer::Software); }
  ALWAYS_INLINE bool IsRunaheadEnabled() const { return (runahead_frames > 0); }

  ALWAYS_INLINE PGXPMode GetPGXPMode()
  {
    return gpu_pgxp_enable ? (gpu_pgxp_cpu ? PGXPMode::CPU : PGXPMode::Memory) : PGXPMode::Disabled;
  }

  ALWAYS_INLINE bool UsingPGXPDepthBuffer() const { return gpu_pgxp_enable && gpu_pgxp_depth_buffer; }
  ALWAYS_INLINE bool UsingPGXPCPUMode() const { return gpu_pgxp_enable && gpu_pgxp_cpu; }
  ALWAYS_INLINE float GetPGXPDepthClearThreshold() const { return gpu_pgxp_depth_clear_threshold * 4096.0f; }
  ALWAYS_INLINE void SetPGXPDepthClearThreshold(float value) { gpu_pgxp_depth_clear_threshold = value / 4096.0f; }

  ALWAYS_INLINE bool IsUsingFastmem() const
  {
    return (cpu_fastmem_mode != CPUFastmemMode::Disabled && cpu_execution_mode == CPUExecutionMode::Recompiler &&
            !cpu_recompiler_memory_exceptions);
  }

  float GetDisplayAspectRatioValue() const;

  ALWAYS_INLINE static bool IsPerGameMemoryCardType(MemoryCardType type)
  {
    return (type == MemoryCardType::PerGame || type == MemoryCardType::PerGameTitle ||
            type == MemoryCardType::PerGameFileTitle);
  }
  bool HasAnyPerGameMemoryCards() const;

  static void CPUOverclockPercentToFraction(u32 percent, u32* numerator, u32* denominator);
  static u32 CPUOverclockFractionToPercent(u32 numerator, u32 denominator);

  void SetCPUOverclockPercent(u32 percent);
  u32 GetCPUOverclockPercent() const;
  void UpdateOverclockActive();

  static constexpr u32 DEFAULT_DMA_MAX_SLICE_TICKS = 1000, DEFAULT_DMA_HALT_TICKS = 100, DEFAULT_GPU_FIFO_SIZE = 16,
                       DEFAULT_GPU_MAX_RUN_AHEAD = 128, DEFAULT_VRAM_WRITE_DUMP_WIDTH_THRESHOLD = 128,
                       DEFAULT_VRAM_WRITE_DUMP_HEIGHT_THRESHOLD = 128;

  void Load(SettingsInterface& si);

  static std::optional<LogLevel> ParseLogLevelName(const char* str);
  static const char* GetLogLevelName(LogLevel level);

  static std::optional<ConsoleRegion> ParseConsoleRegionName(const char* str);
  static const char* GetConsoleRegionName(ConsoleRegion region);
  static const char* GetConsoleRegionDisplayName(ConsoleRegion region);

  static std::optional<DiscRegion> ParseDiscRegionName(const char* str);
  static const char* GetDiscRegionName(DiscRegion region);
  static const char* GetDiscRegionDisplayName(DiscRegion region);

  static std::optional<CPUExecutionMode> ParseCPUExecutionMode(const char* str);
  static const char* GetCPUExecutionModeName(CPUExecutionMode mode);
  static const char* GetCPUExecutionModeDisplayName(CPUExecutionMode mode);

  static std::optional<CPUFastmemMode> ParseCPUFastmemMode(const char* str);
  static const char* GetCPUFastmemModeName(CPUFastmemMode mode);
  static const char* GetCPUFastmemModeDisplayName(CPUFastmemMode mode);

  static std::optional<GPURenderer> ParseRendererName(const char* str);
  static const char* GetRendererName(GPURenderer renderer);
  static const char* GetRendererDisplayName(GPURenderer renderer);

  static std::optional<GPUTextureFilter> ParseTextureFilterName(const char* str);
  static const char* GetTextureFilterName(GPUTextureFilter filter);
  static const char* GetTextureFilterDisplayName(GPUTextureFilter filter);

  static std::optional<GPUDownsampleMode> ParseDownsampleModeName(const char* str);
  static const char* GetDownsampleModeName(GPUDownsampleMode mode);
  static const char* GetDownsampleModeDisplayName(GPUDownsampleMode mode);

  static std::optional<DisplayCropMode> ParseDisplayCropMode(const char* str);
  static const char* GetDisplayCropModeName(DisplayCropMode crop_mode);
  static const char* GetDisplayCropModeDisplayName(DisplayCropMode crop_mode);

  static std::optional<DisplayAspectRatio> ParseDisplayAspectRatio(const char* str);
  static const char* GetDisplayAspectRatioName(DisplayAspectRatio ar);

  static std::optional<ControllerType> ParseControllerTypeName(const char* str);
  static const char* GetControllerTypeName(ControllerType type);

  static std::optional<MemoryCardType> ParseMemoryCardTypeName(const char* str);
  static const char* GetMemoryCardTypeName(MemoryCardType type);

  static std::optional<MultitapMode> ParseMultitapModeName(const char* str);
  static const char* GetMultitapModeName(MultitapMode mode);

  // Default to D3D11 on Windows as it's more performant and at this point, less buggy.
#ifdef _WIN32
  static constexpr GPURenderer DEFAULT_GPU_RENDERER = GPURenderer::HardwareD3D11;
#else
  static constexpr GPURenderer DEFAULT_GPU_RENDERER = GPURenderer::HardwareOpenGL;
#endif
  static constexpr GPUTextureFilter DEFAULT_GPU_TEXTURE_FILTER = GPUTextureFilter::Nearest;
  static constexpr GPUDownsampleMode DEFAULT_GPU_DOWNSAMPLE_MODE = GPUDownsampleMode::Disabled;
  static constexpr ConsoleRegion DEFAULT_CONSOLE_REGION = ConsoleRegion::Auto;
  static constexpr float DEFAULT_GPU_PGXP_DEPTH_THRESHOLD = 300.0f;

#ifdef WITH_RECOMPILER
  static constexpr CPUExecutionMode DEFAULT_CPU_EXECUTION_MODE = CPUExecutionMode::Recompiler;
#ifdef WITH_MMAP_FASTMEM
  static constexpr CPUFastmemMode DEFAULT_CPU_FASTMEM_MODE = CPUFastmemMode::MMap;
#else
  static constexpr CPUFastmemMode DEFAULT_CPU_FASTMEM_MODE = CPUFastmemMode::LUT;
#endif
#else
  static constexpr CPUExecutionMode DEFAULT_CPU_EXECUTION_MODE = CPUExecutionMode::CachedInterpreter;
  static constexpr CPUFastmemMode DEFAULT_CPU_FASTMEM_MODE = CPUFastmemMode::Disabled;
#endif

  static constexpr DisplayCropMode DEFAULT_DISPLAY_CROP_MODE = DisplayCropMode::Overscan;
  static constexpr DisplayAspectRatio DEFAULT_DISPLAY_ASPECT_RATIO = DisplayAspectRatio::Auto;

  static constexpr u8 DEFAULT_CDROM_READAHEAD_SECTORS = 8;

  static constexpr ControllerType DEFAULT_CONTROLLER_1_TYPE = ControllerType::DigitalController;
  static constexpr ControllerType DEFAULT_CONTROLLER_2_TYPE = ControllerType::None;
  static constexpr MemoryCardType DEFAULT_MEMORY_CARD_1_TYPE = MemoryCardType::Libretro;
  static constexpr MemoryCardType DEFAULT_MEMORY_CARD_2_TYPE = MemoryCardType::None;
  static constexpr MultitapMode DEFAULT_MULTITAP_MODE = MultitapMode::Disabled;

  static constexpr LogLevel DEFAULT_LOG_LEVEL = LogLevel::Info;

  // Android doesn't create settings until they're first opened, so we have to override the defaults here.
#ifndef __ANDROID__
  static constexpr bool DEFAULT_FAST_BOOT_VALUE = false;
#else
  static constexpr bool DEFAULT_FAST_BOOT_VALUE = true;
#endif
};

extern Settings g_settings;
