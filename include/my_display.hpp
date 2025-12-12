#include <LovyanGFX.hpp>

// Define a custom class derived from LGFX_Device
class LGFX : public lgfx::LGFX_Device
{
private:
  // --- Panel and Interface Instances ---
  // Panel: 2.4/2.8 inch ILI9341 (240x320)
  lgfx::Panel_ILI9341     _panel_instance;
  
  // Bus: SPI Bus Instance
  lgfx::Bus_SPI           _bus_instance;
  
  // Backlight: PWM Control Instance
  lgfx::Light_PWM         _light_instance;
  
  // Touchscreen: XPT2046 is a common resistive touch controller for ILI9341
  lgfx::Touch_XPT2046     _touch_instance;
  // lgfx::Touch_FT5x06   _touch_instance; // Alternative Capacitive Touch

public:

  // Constructor
  LGFX(void)
  {
    // === 1. BUS Configuration (SPI) ===
    {
      auto cfg = _bus_instance.config();
      
      // Use SPI2_HOST or SPI3_HOST for newer chips like ESP32-C6 (instead of deprecated VSPI_HOST)
      cfg.spi_host    = SPI2_HOST;        // Using SPI2 (formerly VSPI)
      cfg.spi_mode    = 0;                // SPI communication mode (0 ~ 3)
      cfg.freq_write  = 40000000;         // SPI clock for writing (40MHz for ILI9341)
      cfg.freq_read   = 16000000;         // SPI clock for reading
      cfg.spi_3wire   = true;             // true for 3-wire SPI (MOSI used for read/write)
      cfg.use_lock    = true;             // Enable transaction lock
      cfg.dma_channel = SPI_DMA_CH_AUTO;  // Use auto DMA channel
      
      // Pin assignments (Ensure these match your actual wiring!)
      cfg.pin_sclk    = 18;               // SCLK pin
      cfg.pin_mosi    = 23;               // MOSI pin
      cfg.pin_miso    = 19;               // MISO pin (-1 = disable, but recommended for read/touch)
      cfg.pin_dc      = 2;               // D/C (Data/Command) pin
      
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    // === 2. PANEL Configuration (ILI9341) ===
    {
      auto cfg = _panel_instance.config();
      
      // Panel Control Pins
      cfg.pin_cs      = 15;              // Chip Select pin
      cfg.pin_rst     = 4;              // Reset pin
      cfg.pin_busy    = -1;              // Busy pin (not typically used for ILI9341)

      // Panel Dimensions and Offsets
      cfg.panel_width      = 240;        // Actual display width
      cfg.panel_height     = 320;        // Actual display height
      cfg.offset_x         = 0;          // X offset
      cfg.offset_y         = 0;          // Y offset
      cfg.offset_rotation  = 1;          // Rotation offset (0-7)
      
      // Read/Write Settings
      cfg.readable         = true;       // Enable reading from the panel
      cfg.invert           = false;      // Don't invert colors
      cfg.rgb_order        = false;      // RGB order (true for BGR)
      cfg.bus_shared       = true;       // Bus is shared with touch/SD card
      
      _panel_instance.config(cfg);
    }

    // === 3. BACKLIGHT Configuration (PWM) ===
    { 
      auto cfg = _light_instance.config();
      
      cfg.pin_bl      = 22;              // Backlight pin (PWM capable)
      cfg.invert      = false;           // No inversion
      cfg.freq        = 44100;           // PWM frequency
      cfg.pwm_channel = 7;               // PWM channel (0-15)

      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);
    }


    // Set the panel instance to the main LGFX object
    setPanel(&_panel_instance);
  }
};

// Create the global instance that you will use in your sketch
LGFX display;