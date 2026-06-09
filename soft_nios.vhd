library ieee;
use ieee.std_logic_1164.all;

entity soft_nios is
	port (
		CICLONE2_CLK1_50 : in std_logic;
		KEY              : in std_logic_vector (1 downto 0);
		LEDS             : out std_logic_vector (9 downto 0);
		SW               : in std_logic_vector (15 downto 0); -- Chaves
        
        -- Displays de resultados
		HEX0             : out std_logic_vector (6 downto 0);
		HEX1             : out std_logic_vector (6 downto 0);
		HEX2             : out std_logic_vector (6 downto 0);

        -- Displays da entrada B
		HEX4             : out std_logic_vector (6 downto 0);
		HEX5             : out std_logic_vector (6 downto 0);

        -- Displays da entrada A
		HEX6             : out std_logic_vector (6 downto 0);
		HEX7             : out std_logic_vector (6 downto 0)
	);
end entity;

architecture nios of soft_nios is
	component nios_ii is
		port (
			clk_clk         : in std_logic := 'X'; 
			reset_reset_n   : in std_logic := 'X'; 
			leds_export     : out std_logic_vector (9 downto 0);
			switches_external_connection_export : in std_logic_vector (15 downto 0);
			hex0_external_connection_export     : out std_logic_vector (6 downto 0);
			hex1_external_connection_export     : out std_logic_vector (6 downto 0);
         hex2_external_connection_export     : out std_logic_vector (6 downto 0);

			hex4_external_connection_export     : out std_logic_vector (6 downto 0);
			hex5_external_connection_export     : out std_logic_vector (6 downto 0);
			hex6_external_connection_export     : out std_logic_vector (6 downto 0);
         hex7_external_connection_export     : out std_logic_vector (6 downto 0)

		);
	end component nios_ii;

begin
	u0 : component nios_ii
	port map (
		clk_clk         => CICLONE2_CLK1_50, 
		reset_reset_n   => KEY(0), 
		leds_export     => LEDS,
		switches_external_connection_export => SW,
		hex0_external_connection_export     => HEX0,
		hex1_external_connection_export     => HEX1,
		hex2_external_connection_export     => HEX2,

		hex4_external_connection_export     => HEX4,
		hex5_external_connection_export     => HEX5,
		hex6_external_connection_export     => HEX6,
      hex7_external_connection_export     => HEX7
	);
	
end architecture;
