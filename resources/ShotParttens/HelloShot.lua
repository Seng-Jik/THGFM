ShootBullet(eneX,eneY,ene_plr_Angle,5,300,0)
bltSpd = 5

SelectFirstBullet()


local s = SelectFirstBullet()
if s ~= -1 then
    repeat
        bltAngle= bltAngle + 0.1
        local s = NextBullet()
    until s == -1
end

