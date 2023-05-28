import {
  Avatar,
  Box,
  Button,
  Card,
  Divider,
  IconButton,
  Sheet,
  Stack,
  Typography,
} from '@mui/joy'
import LogoutIcon from '@mui/icons-material/Logout'
import { useEffect, useState } from 'react'
import {
  SummaryProps,
  UserInfoProps,
  getProfileInfo,
  getThisMonthSummary,
  logout,
} from '../utils/datafetch'
import { getToken, setToken } from '../utils/http'

export const Profile: React.FC = () => {
  const [info, setInfo] = useState<UserInfoProps>()
  const [monthSummery, setMonthSummery] = useState<SummaryProps>()
  useEffect(() => {
    getProfileInfo()
      .then((res) => {
        setInfo(res)
      })
      .catch((err) => {
        console.log(err)
      })
    getThisMonthSummary()
      .then((res) => {
        setMonthSummery(res)
      })
      .catch((err) => {
        console.log(err)
      })
  }, [])
  return (
    <>
      <Stack gap={2}>
        <Box display={'flex'} gap={2}>
          <Avatar size='lg' />
          <Box>
            <Typography level='h5'>{info?.username}</Typography>
            <Typography level='body3'>last login time: today</Typography>
          </Box>
          <IconButton
            sx={{ ml: 'auto' }}
            variant='plain'
            color='neutral'
            onClick={() => {
              logout().then((res) => {
                if (res) {
                  setToken('')
                }
              })
            }}
          >
            <LogoutIcon />
          </IconButton>
        </Box>
        <Divider />
        <Card variant='outlined'>
          <Typography level='h2' fontSize={'lg'}>
            Assets
          </Typography>
          <Divider
            sx={{
              mt: 2,
            }}
          />
          <Box display={'flex'} mt={2}>
            <Box>
              <Typography level='body3'>My assets</Typography>
              <Typography level='body1'>¥{info?.balance}</Typography>
            </Box>
            <Box ml={'auto'}>
              <Typography level='body3'>My liability</Typography>
              <Typography level='body1'>¥11.2</Typography>
            </Box>
          </Box>
        </Card>
        <Card variant='outlined'>
          <Typography level='h2' fontSize={'lg'}>
            Month Expenditure
          </Typography>
          <Divider
            sx={{
              mt: 2,
            }}
          />
          <Box display={'flex'} mt={2}>
            <Box>
              <Typography level='body3'>Expenditure</Typography>
              <Typography level='body1'>¥{monthSummery?.expend}</Typography>
            </Box>
            <Box ml={'auto'}>
              <Typography level='body3'>Income</Typography>
              <Typography level='body1'>¥{monthSummery?.income}</Typography>
            </Box>
          </Box>
        </Card>
      </Stack>
    </>
  )
}
