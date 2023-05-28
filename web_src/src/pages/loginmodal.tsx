import {
  Button,
  FormControl,
  FormLabel,
  Input,
  Modal,
  Sheet,
  Stack,
  Typography,
} from '@mui/joy'
import React, { useEffect } from 'react'
import { login } from '../utils/datafetch'
import { getToken, http, setToken } from '../utils/http'

export const LoginModal: React.FC = () => {
  const [open, setOpen] = React.useState(false)
  const [username, setUsername] = React.useState('')
  const [password, setPassword] = React.useState('')
  useEffect(() => {
    http
      .get('/user/verify_token')
      .then((res) => {
        if (res.status !== 200) {
          setOpen(true)
        }
      })
      .catch((err) => {
        setOpen(true)
      })
  }, [])

  return (
    <Modal
      open={open}
      onClose={() => {
        //setOpen(false)
      }}
      sx={{ display: 'flex', justifyContent: 'center', alignItems: 'center' }}
    >
      <Sheet
        sx={{
          width: '80%',

          p: 2,
          borderRadius: 'lg',
        }}
        variant='outlined'
      >
        <Stack gap={2}>
          <Typography level='h1'>Login</Typography>
          <form
            onSubmit={(ev) => {
              ev.preventDefault()
              login(username, password)
                .then((res) => {
                  if (res.token) {
                    setOpen(false)
                    setToken(res.token)
                  } else {
                    alert('login failed')
                  }
                })
                .catch((err) => {
                  //alert(err)
                })
            }}
          >
            <Stack gap={1}>
              <FormControl required>
                <FormLabel>username</FormLabel>
                <Input
                  autoFocus
                  value={username}
                  onChange={(ev) => {
                    setUsername(ev.target.value)
                  }}
                />
              </FormControl>
              <FormControl required>
                <FormLabel>password</FormLabel>
                <Input
                  value={password}
                  onChange={(ev) => {
                    setPassword(ev.target.value)
                  }}
                />
              </FormControl>
              <Button type='submit'>login!</Button>
            </Stack>
          </form>
        </Stack>
      </Sheet>
    </Modal>
  )
}
