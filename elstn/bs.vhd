--    TABLE 0
      case Gray0(5 downto 2) is
        when "0000" => iD0 <= "0000000000000000"; -- 0
        when "0001" => iD0 <= "0000000000000001"; -- 1
        when "0010" => iD0 <= "0000000100000001"; -- 2
        when "0011" => iD0 <= "0000010000100001"; -- 3
        when "0100" => iD0 <= "0001000100010001"; -- 4
        when "0101" => iD0 <= "0010010010010001"; -- 5
        when "0110" => iD0 <= "0010100100101001"; -- 6
        when "0111" => iD0 <= "0100101010010101"; -- 7
        when "1000" => iD0 <= "0101010101010101"; -- 8
        when "1001" => iD0 <= "1010101011010101"; -- 9
        when "1010" => iD0 <= "1011010101101101"; -- 10
        when "1011" => iD0 <= "1101011101101101"; -- 11
        when "1100" => iD0 <= "1110111011101110"; -- 12
        when "1101" => iD0 <= "1111101111011110"; -- 13
        when "1110" => iD0 <= "1111111011111110"; -- 14
        when "1111" => iD0 <= "1111111111111111"; -- 15
      end case;


