library ieee;
use ieee.std_logic_1164.all;

entity exemplo_nios is
	port (
		CICLONE2_CLK1_50 	: in std_logic;
		LEDS 					: out std_logic_vector (9 downto 0);
		KEY 					: in std_logic_vector (1 downto 0)
	);
end entity;

architecture nios of exemplo_nios is
	component nios_ii is
		port (
			clk_clk 			: in std_logic := 'X'; -- clk
			reset_reset_n 	: in std_logic := 'X'; -- reset_n
			leds_export 	: out std_logic_vector (9 downto 0) --export
		);
	end component nios_ii;

begin
	u0 : component nios_ii
	port map (
		clk_clk			=> CICLONE2_CLK1_50, -- clk.clk
		reset_reset_n	=> KEY(0), -- reset.reset_n
		leds_export 	=> LEDS
	);
	
end architecture;