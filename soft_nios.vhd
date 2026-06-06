library ieee;
use ieee.std_logic_1164.all;

entity soft_nios is
	port (
		CICLONE2_CLK1_50 : in std_logic;
		KEY              : in std_logic_vector (1 downto 0);
		LEDS             : out std_logic_vector (9 downto 0);
		SW               : in std_logic_vector (17 downto 0); -- Chaves
		HEX0             : out std_logic_vector (6 downto 0) -- Display
	);
end entity;

architecture nios of soft_nios is
	component nios_ii is
		port (
			clk_clk         : in std_logic := 'X'; 
			reset_reset_n   : in std_logic := 'X'; 
			leds_export     : out std_logic_vector (9 downto 0);
			switches_external_connection_export : in std_logic_vector (17 downto 0);
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
		hex7_external_connection_export     => HEX0
	);
	
end architecture;